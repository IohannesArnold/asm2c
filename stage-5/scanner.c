/* scanner.c  --  code for tokenising the C input stream
 *
 * Copyright (C) 2013 Richard Smith <richard@ex-parrot.com>
 * All rights reserved.
 */

static line = 1;

static
print_msg(fmt, ap) {
    fprintf(stderr, "%d: ", line);
    vfprintf(stderr, fmt, ap);
    fputc('\n', stderr);
}

warning(fmt) {
    print_msg(fmt, &fmt);
}

error(fmt) {
    print_msg(fmt, &fmt);
    exit(1);
}

int_error(fmt) {
    fputs("Internal error: ", stderr);
    vfprintf(stderr, fmt, &fmt);
    fputc('\n', stderr);
    exit(2);
}

/* A version of getchar() that increments LINE if a '\n' is found */
static
getcharl() {
    auto c = getchar();
    if ( c == '\n' ) ++line;
    return c;
}

/* Skips over a C-style comment (the opening / and * are already read). */
static
skip_ccomm() {
    auto c;

    do {
        do c = getcharl();
        while ( c != -1 && c != '*' );
        c = getcharl();
    } while ( c != -1 && c != '/' );

    if ( c == -1 )
        error("Unexpected EOF during comment");
}

/* Skips over whitespace, including comments, and returns the next character
 * without ungetting it. */
static
skip_white() {
    auto c;
    while (1) {
        do c = getcharl();
        while ( isspace(c) ); 

        if (c == '/') {
            auto c2 = getchar();
            if ( c2 != '*' ) {
                ungetchar(c2);
                return c;
            }
            skip_ccomm();
        }
        else return c;
    }
}

/* Is C a valid character to start an identifier (or keyword)? */
static
isidchar1(c) {
    return c == '_' || isalpha(c);
}

/* Is C a valid character to continue an identifier (or keyword)? */
static
isidchar(c) {
    return c == '_' || isalnum(c);
}

/* Check whether the null-terminated string, NODE->str, is a keyword, and 
 * if so set NODE->type to the keyword token (which is a multicharacter 
 * literal containing at most the first four characters of the keyword, 
 * e.g. 'whil' for "while"); otherwise set NODE->type = 'id' for an 
 * identifier.   Returns NODE. */
static
chk_keyword(node) {
    /* Argument is:  struct node { int type; char str[]; } */
    
    auto keywords[15] = {
        /* 'do' and 'if' have an extra NUL character to pad them to 4 bytes
         * for casting to an int (i.e. a multicharacter literal). */
        "auto", "break", "case", "continue", "default", "do\0", "else", 
        "extern", "goto", "if\0", "return", "static", "switch", "while", 0
    };

    auto i = 0, str = node + 4;
    while ( keywords[i] && strcmp(keywords[i], str) != 0 )
        ++i;

    if ( keywords[i] ) {
        /* Change the id node to an op node. */
        node[0] = *keywords[i];
        node[1] = node[2] = node[3] = node[4] = 0;
    }
    
    return node;
}

/* Create a node, NODE, which will be returned; read a word (i.e. identifier 
 * or keyword) starting with C (which has already been checked by isidchar1) 
 * into NODE->str, set NODE->type, and return NODE. */
static
get_word(c) {
    auto i = 0, len = 32;  /* Len must be >= 16, which is the op node size */
    auto node = malloc(4 + len);  /* struct node { int type; char str[]; } */
    auto str = node + 4;

    node[0] = 'id';
    lchar( str, i++, c );
    while (1) { 
        while ( i < len && isidchar( c = getchar() ) )
            lchar( str, i++, c );
        if ( i < len )
            break;
        len *= 2;
        node = realloc( node, 4 + len );
        str = node + 4;
    } 

    lchar( str, i, 0 );
    ungetchar(c);
    return chk_keyword( node );
}


/* Read a number (in octal, hex or decimal) starting with digit C into BUF, 
 * which must be 16 bytes long (or longer). */
static
read_number(buf, c) {
    auto i = 0;
    if ( c == '0' ) {
        lchar( buf, i++, c );
        c = getchar();
        /* Is it hex? */
        if ( c == 'x' ) {
            lchar( buf, i++, c );
            do lchar( buf, i++, c );
            while ( i < 16 && isxdigit( c = getchar() ) );
        }
        /* Is it octal? */
        else if ( isdigit(c) ) {
            do lchar( buf, i++, c );
            while ( i < 16 && isdigit( c = getchar() ) );
        }
        /* else it's a literal zero */
    }
    /* It must be a decimal */
    else {
        do lchar( buf, i++, c );
        while ( i < 16 && isdigit( c = getchar() ) );
    }
    if ( i == 16 )
        error("Overflow reading number");
    
    lchar( buf, i, 0 );
    ungetchar(c);
}

/* Create a node, NODE, which will be returned; read a number (oct / hex / dec)
 * starting with character C (which has already been checked by isdigit), 
 * parse it into NODE->val, set NODE->type, and return NODE. */
static
get_number(c) {
    auto buf[16], nptr;
    auto node = malloc(8); /* struct node { int type; int val; }; */

    read_number( buf, c );
    node[0] = 'num';
    node[1] = strtol( buf, &nptr, 0 );
    if ( rchar(nptr, 0) )
        error("Unexpected character '%c' in string \"%s\"",
              rchar(nptr, 0), buf);
    
    return node;
}

/* Test whether the two characters in the multicharacter literal, OP, is a 
 * valid lexical representation of an operator.  So '[]' does not  */
static 
is_2charop(op) {
    /* TODO: digraphs, trigraphs, and '->' */
    auto mops[19] = { 
        /* 0     1     2     3     4     5     6     7     8     9 */
        '++', '--', '<<', '>>', '<=', '>=', '==', '!=', '&&', '||',
        '*=', '%=', '/=', '+=', '-=', '&=', '|=', '^=',  0 
    };

    /* Search for two-character operator name in the mops[] list, above */
    auto i = 0;
    while ( mops[i] && mops[i] != op )
        ++i;

    return mops[i] ? 1 : 0;
}

/* struct node { int type; node* op0; node* op1; node* op2; node* op3 } */
node_new(type) {
    /* For binary operators, op0 is the lhs and op1 the rhs; for unary prefix
     * operators, only op0 is used; and for unary postfix only op1 is used. 
     * 
     * The scanner never reads a ternary operator (because ?: has two separate
     * lexical elements), but we generate '?:' nodes in the expression parser
     * and want a uniform interface.  Similarly, the 'for' node is a quaternary
     * "operator" (init, test, incr, stmt). */
    auto n = malloc(20);

    n[0] = type;

    /* The operands will get filled in by the parser */
    n[1] = n[2] = n[3] = n[4] = 0;

    return n;
}

/* Read an operator, starting with character C, and return a node with
 * NODE->type set to the operator as a multicharacter literal. */
static
get_multiop(c) {
    /* Fetch the second character */
    auto c2 = getchar();
    if ( c2 == -1 ) return c;
    else lchar( &c, 1, c2 );

    /* If it's not a two-character operator, it must be a single character 
     * operator -- node that the only three-character operators and two-
     * character operators with an extra character appended.  
     * TODO: '...' breaks that assumption. */
    if ( !is_2charop(c) ) {
        ungetchar( rchar( &c, 1 ) );
        return node_new( rchar( &c, 0 ) );
    }

    /* Is it a <<= or >>= operator? */
    if ( c == '<<' || c == '>>' ) {
        c2 = getchar();
        if ( c2 == '=' ) lchar( &c, 2, c2 );
        else ungetchar(c2);
    }

    return node_new(c);
}

/* Read a string or character literal into VALUE, beginning with quote mark, 
 * Q, and return the appropriate token type ('str' or 'chr'). */
static
get_qlit(q) {
    auto i = 0, len = 32, c;
    auto node = malloc(4 + len); /* struct node { int type; char str[]; } */
    auto str = node + 4;

    lchar( str, i++, q );
    while (1) {
        while ( i < len-1 && (c = getchar()) != -1 && c != q ) {
            lchar( str, i++, c );
            if ( i < len-1 && c == '\\' ) {
                if ( (c = getchar()) == -1 ) break;
                lchar( str, i++, c );
            }
        }

        if ( c == -1 )
            error("Unexpected EOF during %s literal",
                  q == '"' ? "string" : "character");

        if ( i < len-1 )
            break;
        len *= 2;
        node = realloc( node, 4 + len );
        str = node + 4;
    }

    /* We can only reach here if i < len-1, so no need to check for overflow */
    lchar( str, i++, q );
    lchar( str, i++, 0 );
    node[0] = q == '"' ? 'str' : 'chr';
    return node;
}

/* Contains the token most recently read by next() */
token = 0;

/* Read the next lexical element as a syntax tree node */
next() {
    auto c = skip_white();
    if ( c == -1 )
        token = 0;
    else if ( isidchar1(c) )
        token = get_word(c);
    else if ( isdigit(c) )
        token = get_number(c);
    else if ( c == '\'' || c == '"' )
        token = get_qlit(c);
    else 
        token = get_multiop(c);
    return token;
}

/* Test whether token type OP is an operator in the syntax tree. */
is_op(op) {
    /* TODO: '.', ',' */
    return strnlen(&op, 4) == 1 && strchr("&*+-~!/%<>^|=", op)
        || is_2charop(op) || op == '?:' || op == '[]' 
        || op == '>>=' || op == '<<=';
}

is_stmt_op(op) {
    return op == 'if' || op == 'do' || op == 'whil' || op == 'retu' 
        || op == 'brea' || op == 'cont';
}

/* Unallocate a node */
free_node(node) {
    if (!node) return;

    /* As a safety measure, if it's the current node, unset TOKEN. */
    if ( node == token ) token = 0;

    /* Recurse the syntax tree for an operator freeing its operands. */
    if ( is_op( node[0] ) || is_stmt_op( node[0] ) ) {
        auto i = 0;
        while ( i < 4 )
            free_node( node[ 1 + i++ ] );
    }

    else if ( node[0] == '()' || node[0] == '{}' ) {
        auto i = 0;
        while ( i < node[2] )
            free_node( node[ 3 + i++ ] );
        free_node( node[1] );
    }

    free(node);
}

/* Skip over a piece of syntax, deallocating its node */
skip_node(type) {
    if (!token)
        error("Unexpected EOF when expecting '%Mc'", type);

    else if (token[0] != type)
        error("Expected a '%Mc'", type);
    
    free_node(token);
    return next();
}

/* Take ownership of the current node, and call next() */
take_node() {
    auto node = token;
    next();
    return node;
}

/* Require P to be non-null, and give an 'unexpected EOF' error if it is not.
 * Returns P. */
req_token(p) {
    if (!p) error("Unexpected EOF");
    return p;
}

