/* codegen.c  --  code generation pass
 *
 * Copyright (C) 2013 Richard Smith <richard@ex-parrot.com>
 * All rights reserved.
 */

/* The clabel_cnt is for .LC labels, used for string literals. */
static clabel_cnt = 0;

static
new_clabel() {
    return ++clabel_cnt;
}

/* The label_cnt is for .L labels, used for local jumps. */
static label_cnt = 0;

static
new_label() {
    return ++label_cnt;
}

/* Bytes of local variables on the stack. */
static frame_size = 0;


/* Code for leaf nodes in the expression parse tree: constants, strings,
 * identifers, and so on.  */
static
leaf_code(stream, node, need_lval) {
    if ( node[0] == 'num' || node[0] == 'chr' || node[0] == 'str' ) {
        if (need_lval) error("Literal used as an lvalue");

        if (node[0] == 'num') load_num(stream, node[2]);
        else if (node[0] == 'chr') load_chr(stream, node[2]);
        else if (node[0] == 'str') load_str(stream, &node[2], new_clabel());
    }

    else if ( node[0] == 'id' ) {
        auto off, is_lval = lookup_sym( &node[2], &off );
        auto need_addr = (is_lval == need_lval);
        if (need_lval && !is_lval) 
            error("Non-lvalue identifier where lvalue is required");
        if (!off) load_symbol(stream, &node[2], need_addr);
        else load_local(stream, off, need_addr);
    }

    else int_error("Unknown token '%Mc' in parse tree", node[0]);
}

static
unary_post(stream, node, need_lval) {
    expr_code( stream, node[2], 1 );

    if      ( node[0] == '++' ) postfix_inc(stream);
    else if ( node[0] == '--' ) postfix_dec(stream);
    else int_error( "Unknown operator: '%Mc'", node[0] );
}

static
unary_pre(stream, node, need_lval) {
    auto op = node[0];
    auto op_needs_lv = op == '++' || op == '--' || op == '&';
    expr_code( stream, node[2], op_needs_lv );

    if      ( op == '+'  ) ;
    else if ( op == '-'  ) arith_neg(stream);
    else if ( op == '~'  ) bit_not(stream);
    else if ( op == '!'  ) logic_not(stream);
    else if ( op == '&'  ) ;
    else if ( op == '*'  ) dereference(stream, need_lval);
    else if ( op == '++' ) increment(stream);
    else if ( op == '--' ) decrement(stream);
    else int_error("Unknown operator: '%Mc'", op);
}

static
logical_bin(stream, node) {
    auto l = new_label();

    expr_code( stream, node[2], 0 );
    if ( node[0] == '&&' ) branch_ifz( stream, l );
    else branch_ifnz( stream, l );

    expr_code( stream, node[3], 0 );
    emit_label( stream, l );
    cast_bool( stream );
}

static
conditional(stream, node) {
    auto l1 = new_label(), l2 = new_label();

    expr_code( stream, node[2], 0 );

    branch_ifz( stream, l1 );
    expr_code( stream, node[3], 0 );
    branch( stream, l2 );
    
    emit_label( stream, l1 );
    expr_code( stream, node[4], 0 );
    emit_label( stream, l2 );
}

static
binary_op(stream, node, need_lval) {
    auto op = node[0];
    auto op_needs_lv = is_assop(op) || op == '[]';
    expr_code( stream, node[2], op_needs_lv );

    asm_push( stream );
    expr_code( stream, node[3], 0);

    if      ( op == '[]'  ) pop_subscr(stream, need_lval);
    else if ( op == '*'   ) pop_mult(stream, 0);
    else if ( op == '/'   ) pop_div(stream, 0);
    else if ( op == '%'   ) pop_mod(stream, 0);
    else if ( op == '+'   ) pop_add(stream, 0);
    else if ( op == '-'   ) pop_sub(stream, 0);
    else if ( op == '<<'  ) pop_lshift(stream, 0);
    else if ( op == '>>'  ) pop_rshift(stream, 0);
    else if ( op == '<'   ) pop_lt(stream);
    else if ( op == '>'   ) pop_gt(stream);
    else if ( op == '<='  ) pop_le(stream);
    else if ( op == '>='  ) pop_ge(stream);
    else if ( op == '=='  ) pop_eq(stream);
    else if ( op == '!='  ) pop_ne(stream);
    else if ( op == '&'   ) pop_bitand(stream, 0);
    else if ( op == '|'   ) pop_bitor(stream, 0);
    else if ( op == '^'   ) pop_bitxor(stream, 0);

    else if ( op == '='   ) pop_assign(stream);
    else if ( op == '*='  ) pop_mult(stream, 1);
    else if ( op == '/='  ) pop_div(stream, 1);
    else if ( op == '%='  ) pop_mod(stream, 1);
    else if ( op == '+='  ) pop_add(stream, 1);
    else if ( op == '-='  ) pop_sub(stream, 1);
    else if ( op == '<<=' ) pop_lshift(stream, 1);
    else if ( op == '>>=' ) pop_rshift(stream, 1);
    else if ( op == '&='  ) pop_bitand(stream, 1);
    else if ( op == '|='  ) pop_bitor(stream, 1);
    else if ( op == '^='  ) pop_bitxor(stream, 1);

    else int_error("Unknown operator: '%Mc'", op);

    /* The assignment operators are implemented to return lvalues, but
     * the C standard says they're not lvalues.  (The C++ standard has
     * them as lvalues.)  */
    if ( is_assop(op) )
        dereference(stream, 0);
}

static
opexpr_code(stream, node, need_lval) {
    auto op = node[0];

    /* The following operators result in an lvalue: *, [] */
    if (need_lval && op != '*' && op != '[]')
        error("Non-lvalue expression used as lvalue");

    /* Unary prefix operators */
    if ( node[1] == 1 )
        unary_pre( stream, node, need_lval );

    /* Unary postfix operators (marked binary in the tree) */
    else if ( op == '++' || op == '--' )
        unary_post( stream, node, need_lval );

    /* These operators are handled separately because of short-circuiting */
    else if ( op == '&&' || op == '||' )
        logical_bin( stream, node );
    
    /* Binary operators */
    else if ( node[1] == 2 )
        binary_op( stream, node, need_lval );

    /* The ternary operator also short-circuits */
    else if ( op == '?:' )
        conditional( stream, node );

    else
        int_error( "Unknown operator: '%Mc'", node[0] );
}

static
do_call(stream, node, need_lval) {
    auto args = node[1] - 1, i = args;
    while ( i ) {
        expr_code( stream, node[ 3 + --i ], 0 );
        asm_push( stream );
    }
    asm_call( stream, &node[2][2], args );
}

static
expr_code(stream, node, need_lval) {
    auto op = node[0];
    if ( op == '()' )
        do_call( stream, node, need_lval );
    else if ( is_op(node[0]) )
        opexpr_code( stream, node, need_lval );
    else
        return leaf_code( stream, node, need_lval );
}

static
return_stmt(stream, node, ret) {
    if ( node[2] ) expr_code( stream, node[2], 0 );
    branch( stream, ret );
}

static
if_stmt(stream, node, brk, cont, ret) {
    auto l1 = new_label(), l2 = l1;

    expr_code( stream, node[2], 0 );
    
    branch_ifz( stream, l1 );
    expr_code( stream, node[3], 0 );

    if ( node[4] ) {
        l2 = new_label();
        branch( stream, l2 );
        emit_label( stream, l1 );
        expr_code( stream, node[4], 0 );
    }

    emit_label( stream, l2 );
}

static
while_stmt(stream, node, brk, cont, ret) {
    cont = new_label();
    emit_label( stream, cont );

    expr_code( stream, node[2], 0 );
    brk = new_label();
    branch_ifz( stream, brk );

    stmt_code( stream, node[3], brk, cont, ret );
    branch( stream, cont );
    emit_label( stream, brk );
}

static
do_stmt(stream, node, brk, cont, ret) {
    auto start = new_label();
    emit_label( stream, start );

    cont = new_label();
    brk = new_label();
    
    stmt_code( stream, node[2], brk, cont, ret );

    emit_label( stream, cont );
    expr_code( stream, node[3], 0 );
    branch_ifnz( stream, start );
    emit_label( stream, brk );
}

static
type_size(type) {
    if (type && type[0] == '[]')
        return 4 * type[3][2];
    else
        return 4;
}

static
auto_stmt(stream, node) {
    auto i = 0;
    while ( i < node[1] ) {
        auto decl = node[ 2 + i++ ];
        auto sz = type_size( decl[2] );

        /* Arrays are currently the only object that's not an lvalue */
        auto is_lval = !( decl[2] && decl[2][0] == '[]' );

        frame_size += sz;
        if ( decl[3][0] != 'id' ) int_error("Expected identifier in auto decl");
        save_sym( &decl[3][2], -frame_size, is_lval, sz );

        if ( decl[4] ) {
            auto init = decl[4];
            if ( init[0] == '{}' ) {
                /* Check for an initialisation list that's not long enough.
                 * If it's not, the standard requires zero initialisation of
                 * the extra members. C99:6.7.8/21 */
                if ( 4 * init[1] < sz )
                    push_n_zero( stream, sz/4 - init[1] );

                auto j = init[1];
                while ( j ) {
                    expr_code( stream, init[ 2 + --j ], 0 );
                    asm_push( stream );
                }
            }
            /* Scalar initialisation */
            else {
                expr_code( stream, init, 0 );
                asm_push( stream );
            }
        }
        /* If no intialiser, then just allocate memory */
        else alloc_stack( stream, sz );
    }
}

static
stmt_code(stream, node, brk, cont, ret) {
    /* Handle the null expression. */
    if ( !node ) return;

    auto op = node[0];

    if      ( op == 'auto' ) auto_stmt( stream, node );

    else if ( op == 'brea' ) branch( stream, brk );
    else if ( op == 'cont' ) branch( stream, cont ); 
    else if ( op == 'retu' ) return_stmt( stream, node, ret );

    else if ( op == 'if'   ) if_stmt( stream, node, brk, cont, ret );
    else if ( op == 'whil' ) while_stmt( stream, node, brk, cont, ret );
    else if ( op == 'do'   ) do_stmt( stream, node, brk, cont, ret );
    
    else if ( op == '{}'   ) do_block( stream, node, brk, cont, ret );
    else                     expr_code( stream, node, 0 );
}

static
do_block(stream, node, brk, cont, ret) {
    auto i = 0, sz;
    new_scope();
    while ( i < node[1] )
        stmt_code( stream, node[ 2 + i++ ], brk, cont, ret );
    sz = end_scope();
    clear_stack( stream, sz );
    frame_size -= sz;
}

static
storage(stream, storage, name) {
    if (storage == 'stat')
        local_decl(stream, name);
    else
        globl_decl(stream, name);
}

static
fn_decl(stream, name, decl, block) {
    auto ret = new_label();

    frame_size = 0;
    new_scope();

    /* Inject the parameters into the scope */
    auto i = 1;
    while ( i < decl[1] ) {
        auto n = decl[ 2 + i++ ];
        save_sym( &n[2], 4*i, 1, 4 );
    }

    prolog(stream, name);
    do_block(stream, block, -1, -1, ret);
    emit_label( stream, ret );
    epilog(stream);

    end_scope();
}

static
int_decl(stream, name, init ) {
    data_decl(stream, name);
    if (!init)
        int_decl_n(stream, 0);
    else if (init[0] == 'num')
        int_decl_n(stream, init[2]);
    else
        int_decl_s(stream, &init[2]);
}

static
array_decl(stream, name, type, init) {
    auto sz = type_size( type );
    data_decl(stream, name);

    if (init) {
        auto i = 0;
        while ( i < init[1] ) {
            auto ival = init[ 2 + i++ ];
            if (ival[0] == 'num')
                int_decl_n(stream, ival[2]);
            else
                int_decl_s(stream, &ival[2]);
        }

        if ( 4 * init[1] < sz )
            zero_direct( stream, sz - 4*init[1] );
    }
}

codegen(stream, node) {
    auto i = 0;
    while ( i < node[1] ) {
        auto decl = node[ 2 + i++ ];
        auto type = decl[2], name = &decl[3][2], init = decl[4];

        storage(stream, node[0], name);

        if (!type) /* all scalars and only scalars are currently typeless */
            int_decl( stream, name, init );
        else if (type[0] == '()')
            fn_decl( stream, name, type, init );
        else if (type[0] == '[]')
            array_decl( stream, name, type, init );
        else 
            int_error("Unexpected node in declaration");
    }
}
