/* node.b  --  low-level code for manipulating AST nodes
 * Copyright (C) 2013-2019 Richard Smith <richard@ex-parrot.com>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

static
rc_count = 0;

/* Allocate SZ bytes of memory, adding a reference-counted header. */
static
rc_alloc(sz) {
    auto ptr = malloc(8 + sz);
    ++rc_count;
    ptr[0] = 1;  /* the reference count */
    ptr[1] = sz; /* the capacity */
    return &ptr[2];
}

/* Unconditionally unallocate PTR which is memory allocated by rc_alloc. */
static
rc_free(ptr) {
    --rc_count;
    free( &ptr[-2] );
}

/* Diagnostic routine to check that all nodes have been unallocated. */
rc_done() {
    if (rc_count) {
        extern stderr;
        fprintf(stderr, "Internal error: program leaked %d objects\n",
                rc_count);
    }
}

/* Wrapper around realloc to work with pointers returned by rc_alloc. */
static
rc_realloc(old_ptr, sz) {
    auto new_ptr;
    old_ptr = &old_ptr[-2];

    /* We cannot currently handle reallocating if there are multiple copies.  
     * What should it do?  If the address changes, we need to update all
     * the references, but we cannot do that.  So we'd have to create a unique
     * clone first, but then it's not really shared.  Best to prohibit it. */
    if ( *old_ptr > 1 )
        abort();

    new_ptr = realloc( old_ptr, sz + 8 );
    new_ptr[1] = sz;
    return &new_ptr[2];
}

/* Increment the reference count on a pointer */
add_ref(ptr) {
    ++ptr[-2];
    return ptr;
}

/* Allocate a new node of type TYPE. */
new_node(type, arity) {
    /* struct node { int type; int nops; node* type; node* op[4]; } 
     *
     * For binary operators, op[0] is the lhs and op[1] the rhs; for unary 
     * prefix operators, only op[0] is used; and for unary postfix only 
     * op[1] is used. 
     * 
     * The scanner never reads a ternary operator (because ?: has two separate
     * lexical elements), but we generate '?:' nodes in the expression parser
     * and want a uniform interface.  Similarly, the 'for' node is a quaternary
     * "operator" (init, test, incr, stmt). */
    auto n = rc_alloc(28);

    n[0] = type; n[1] = arity;

    /* The type and payload (operands) will get filled in by the parser */
    memset( &n[2], 0, 20 );

    return n;
}

/* Unallocate a node, NODE, created by new_node(). */
free_node(node) {
    auto i = 0;

    if (!node) return;
    /* Trap for double delete */ 
    else if (node[-2] == 0) abort();
    /* If the reference count doesn't drop to zero, do nothing. */
    else if (--node[-2]) return;

    free_node( node[2] );

    while ( i < node[1] ) 
        free_node( node[ 3 + i++ ] );

    rc_free(node);
}

/* If SIZE is equal to the capacity of NODE, then reallocate it with twice
 * capacity, and return the new node. */
static
grow_node(node, size) {
    /* 12 is the size of the node before the payload. */

    if ( size + 12 == node[-1] ) {
        size *= 2;
        return rc_realloc( node, size + 12 );
    }

    return node;
}

/* Append node N to the vector node V, growing the vector if necessary, 
 * and returning the (possibly reallocated) vector. */
vnode_app( v, n ) {
    v = grow_node(v, v[1] * 4);
    v[ 3 + v[1]++ ] = n;
    return v;
}

/* Returns a pointer to the string payload of a node */
node_str(node) {
    return &node[3];
}

/* Returns the node type.  This only exists to abstract the difference
 * between t[0] (in stage 2) and t->code (in stage 4).  */
node_code(node) {
    return node[0];
}

node_arity(node) {
    return node[1];
}

node_type(node) {
    return node[2];
}

node_op(node, n) {
    return node[3+n];
}

set_code(node, code) {
    node[0] = code;
}

set_arity(node, arity) {
    node[1] = arity;
}

set_type(node, type) {
    node[2] = type;
}

set_op(node, n, op) {
    node[3+n] = op;
}

/* Append character CHR to the payload of the node *NODE_PTR which is treated 
 * as a string with current length *LEN_PTR.  The value of *LEN_PTR is 
 * incremented.  The node may be reallocated. */
node_lchar( node_ptr, len_ptr, chr )
    int *len_ptr;
{
    auto node = *node_ptr;
    node = grow_node( node, *len_ptr );
    lchar( node_str(node), (*len_ptr)++, chr );
    *node_ptr = node;
}

/* Push-back facility doesn't really belong here, but having to keep 
 * compatibility with the stage 2 bc is tricky.
 *
 * This is a struct pb_slot { struct node* token; struct pb_slot* next } *; */
static pb_stack = 0;

pb_empty() {
    return !pb_stack;
}

pb_pop() {
    auto ret = 0;
    if ( pb_stack ) {
        auto old = pb_stack;
        ret = pb_stack[0];
        pb_stack = pb_stack[1];
        free(old);
    }
    return ret;
}

pb_push(token) {
    auto p = malloc(8);
    p[1] = pb_stack;
    pb_stack = p;
    pb_stack[0] = token;
}

/* Allocate a string node and set its payload to STR */
struct node*
new_strnode(code, str)
    char* str;
{
    auto sz = strlen(str) + 1;
    auto node = rc_alloc( 12 + sz );
    set_code( node, code );
    set_arity( node, 0 );
    set_type( node, 0 );
    strcpy( node_str(node), str, sz );
    return node;
}


