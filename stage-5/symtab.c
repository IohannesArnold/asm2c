/* symtab.c  --  code to manipulate the symbol table
 *
 * Copyright (C) 2013 Richard Smith <richard@ex-parrot.com>
 * All rights reserved.
 */

/* This table contains the namespace of ordinary identifiers */
static symtab[3] = { 0, 0, 0 };  /* start, end, end_store */
/*  struct sym_entry {
 *      char sym[12];
 *      int  frame_off;     [3]
 *      int  scope_id;      [4]
 *      int  lval;          [5]
 *      int  size;          [6]
 *  };                              size = 28
 */

/* This table contains the namespace of label names */
static labtab[3] = { 0, 0, 0 };  /* start, end, end_store */
/*  struct lab_entry {
 *      char sym[12];
 *      bool defined;       [3]
 *      int  label_num;     [4]
 *  };                              size = 20
 */ 

static scope_id = 0;

/* Initialise a table */
static
init_tab( tab, entry_size ) {
    auto sz = 1 * entry_size;
    auto p = malloc(sz);
    tab[0] = tab[1] = p;
    tab[2] = p + sz;
}

/* Check whether the symbol table is full, and if so double its capacity. 
 * Copied NAME into the entry, increment the table's end pointer by
 * ENTRY_SIZE, and return the entry pointer.   */
static
grow_tab( tab, entry_size, name ) {
    auto sz = tab[1] - tab[0];
    auto p = tab[0];
    if ( tab[1] == tab[2] ) {
        p = realloc( tab[0], 2*sz );
        tab[0] = p;
        tab[1] = p + sz;
        tab[2] = p + 2*sz;
    }
    strcpy( tab[1], name );
    tab[1] += entry_size;
    return p + sz;
}

/* Lookup NAME in TAB and return a pointer to the table entry, 
 * or NULL if not found. */
static
lookup_tab( tab, entry_size, name ) {
    auto e = tab[0];
    while ( e < tab[1] ) {
        if ( strcmp( e, name ) == 0 )
            return e;
        e += entry_size;
    }
    return 0;
}

init_symtab() {
    init_tab( symtab, 28 );         /* sizeof(sym_entry) */
    init_tab( labtab, 20 );         /* sizeof(lab_entry) */
}

/* Define a symbol, possibly implicitly by calling a function */
save_sym( name, frame_off, lval, sym_size ) {
    auto e = grow_tab( symtab, 28, name );   /* sizeof(sym_entry) */
    e[3] = frame_off;
    e[4] = scope_id;
    e[5] = lval;
    e[6] = sym_size; 
}

/* Define a label, possibly implicitly in the case of a forwards goto. 
 * Return 1 if it is a new label. */
save_label( name, is_defn ) {
    auto e = lookup_tab( labtab, 20, name ); /* sizeof(lab_entry) */
    auto rv = 1;
    if (e) {
        if ( is_defn ) {
            if (e[3]) error( "Multiple definition of label '%s'", name );
            e[3] = is_defn;
        }
        rv = 0;
    }
    else {
        e = grow_tab( labtab, 20, name ); /* sizeof(lab_entry) */
        e[3] = is_defn;
        e[4] = 0;
    }
    return rv;
}

/* Store (and return) the label number for an already-saved label. */
set_label( name, num ) {
    auto e = lookup_tab( labtab, 20, name ); /* sizeof(lab_entry) */
    if (!e) int_error("Trying to set an undefined label");
    if (e[4]) int_error("Trying to set an already set label");
    e[4] = num;
    return num;
}

/* Return the label number for an already-set label. */
get_label( name ) {
    auto e = lookup_tab( labtab, 20, name ); /* sizeof(lab_entry) */
    if (!e) int_error("Trying to set an undefined label");
    if (!e[4]) int_error("Trying to get an unset label");
    return e[4];
}

/* Called on parsing '{' or otherwise starting a new nested scope. */
static
new_scope() {
    ++scope_id;
}

/* Called on parsing '}' or similar to remove symbols from the table.
 * Returns number of bytes that need removing from the stack. */
static
end_scope() {
    auto e = symtab[0], sz = 0;
    while ( e < symtab[1] ) {
        if ( e[4] == scope_id ) break;
        e += 28;    /* sizeof(sym_entry) */
    }

    /* Anything at e or beyond is about to go out of scope.  
     * Add up the size. */
    if ( e < symtab[1] ) {
        auto f = e;
        while ( e < symtab[1] ) {
            sz += e[6];
            e += 28;    /* sizeof(sym_entry) */
        }
        symtab[1] = f;
    }

    --scope_id;
    return sz;
}

/* Return the lvalue flag for the symbol NAME, or 1 if it is 
 * not defined (as we assume external symbols are lvalues).  
 * Also set *OFF_PTR to the symbol table offset of the symbol, or
 * 0 if it is not defined (as 0 is not a valid offset because 
 * 0(%ebp) is the calling frame's base pointer.) */
lookup_sym( name, off_ptr ) {
    auto e = lookup_tab( symtab, 28, name );    /* sizeof(sym_entry) */
    if (e) {
        *off_ptr = e[3];
        return e[5];
    }

    /* Symbol not found */
    *off_ptr = 0;
    return 1;
}

type_size(type) {
    if (type && type[0] == '[]')
        return 4 * type[3][2];
    else
        return 4;
}

/* Bytes of local variables on the stack. */
static frame_sz = 0;

frame_off() {
    return -frame_sz; 
}

decl_var(decl) {
    auto sz = type_size( decl[2] );
    /* Arrays are currently the only object that's not an lvalue */
    auto is_lval = !( decl[2] && decl[2][0] == '[]' );

    frame_sz += sz;
    if ( decl[3][0] != 'id' ) int_error("Expected identifier in auto decl");
    save_sym( &decl[3][2], -frame_sz, is_lval, sz );
    return sz;
}

start_fn(decl) {
    auto i = 1;
    frame_sz = 0;
    new_scope();

    /* Inject the parameters into the scope */
    while ( i < decl[1] ) {
        auto n = decl[ 2 + i++ ];
        if (n[0] != 'id') int_error("Non-identifier found as function param");
        save_sym( &n[2], 4*i, 1, 4 ); /* TODO: type size */
    }
}

end_fn() {
    end_scope();

    /* Drop all labels */
    labtab[1] = labtab[0];
}

start_block() {
    new_scope();
}

end_block() {
    auto sz = end_scope();
    frame_sz -= sz;
    return sz;
}
