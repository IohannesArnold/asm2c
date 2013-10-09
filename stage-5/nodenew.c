/* cppnode.c  --  node.c rewritten to use structs
 *
 * Copyright (C) 2013 Richard Smith <richard@ex-parrot.com>
 * All rights reserved.
 */

/* This disables the errors on incompatibilities with stage-4. 
 * This is safe because this file is never processed with the stage-4
 * compiler (and, indeed, wouldn't compile if were). */
#pragma RBC compatibility 5 

extern stderr;

static
rc_count = 0;

struct rc_node {
    int ref_count, capacity;
};

/* Allocate SZ bytes of memory, adding a reference-counted header. */
static
rc_alloc(sz) {
    struct rc_node* ptr = malloc( sizeof(struct rc_node) + sz );
    ++rc_count;
    ptr->ref_count = 1;
    ptr->capacity = sz;
    return ptr + 1;
}

/* Unconditionally unallocate PTR which is memory allocated by rc_alloc. */
static
rc_free(ptr) {
    /* Somewhat counter-intuitive, but the caller has already decremented
     * the ref_count.  */
    --rc_count;
    free( ptr - sizeof(struct rc_node) );
}

/* Diagnostic routine to check that all nodes have been unallocated. */
rc_done() {
    if (rc_count)
        fprintf(stderr, "Internal error: program leaked %d objects\n",
                rc_count);
}

/* Wrapper around realloc to work with pointers returned by rc_alloc. */
static
rc_realloc(ptr, sz) {
    struct rc_node *old_ptr, *new_ptr;

    if ( !ptr )
        return rc_alloc(sz);
    
    old_ptr = (struct rc_node*)( (unsigned char*)ptr - sizeof(struct rc_node) );

    /* We cannot currently handle reallocating if there are multiple copies.  
     * What should it do?  If the address changes, we need to update all
     * the references, but we cannot do that.  So we'd have to create a unique
     * clone first, but then it's not really shared.  Best to prohibit it. */
    if ( old_ptr->ref_count > 1 )
        abort();

    new_ptr = (struct rc_node*) realloc( old_ptr, sizeof(struct rc_node) + sz );
    new_ptr->ref_count = 1;
    new_ptr->capacity = sz;
    return new_ptr + 1;
}

/* Increment the reference count on a pointer */
add_ref(ptr) {
    struct rc_node* n = ptr - sizeof(struct rc_node);
    ++n->ref_count;
    return ptr;
}

struct node {
    int code;           /* character code for the node, e.g. '+' or 'if'. */
    int arity;          /* the number of nodes in the ops[] array. */
    struct node* type;  /* a node representing the type of the node. */

    /* For binary operators, ops[0] is the lhs and ops[1] the rhs; for unary 
     * prefix operators, only ops[0] is used; and for unary postfix only 
     * ops[1] is used. 
     * 
     * The scanner never reads a ternary operator (because ?: has two separate
     * lexical elements), but we generate '?:' nodes in the expression parser
     * and want a uniform interface.  Similarly, the 'for' node is a quaternary
     * "operator" (init, test, incr, stmt). */
    struct node* ops[4];
};

/* Allocate a new node of code CODE, and arity ARITY. */
new_node(code, arity) {
    struct node* n = rc_alloc( sizeof(struct node) );
    memset( n, 0, sizeof(struct node) );

    /* The type and payload (operands) will get filled in by the parser */
    n->code = code; 
    n->arity = arity;

    return n;
}

/* Unallocate a node, NODE, created by new_node(). */
free_node(node)
    struct node* node;
{
    if (node) {
        struct rc_node* rc = (unsigned char*)node - sizeof(struct rc_node);

        /* Trap for double delete */ 
        if ( rc->ref_count == 0 ) abort();

        /* Only delete if the reference count drops to zero. */
        if ( --rc->ref_count == 0 ) {
            int i;
            for ( i = 0; i < node->arity; ++i ) 
                free_node( node->ops[i] );

            free_node( node->type );
            rc_free(node);
        }
    }
}

/* Expand, if necessary, the storage of NODE.  SIZE is the current size
 * (in bytes) of the node, and EXTRA is the additional space required.
 * If SIZE + EXTRA is greater than the capacity (in bytes) of NODE, then
 * reallocate it with twice (or, if necessary, more) capacity, and return 
 * the new node.  It does not increment the arity of the node.  More 
 * friendly interfaces are provided by vnode_app() and node_lchar(). */
static
struct node*
grow_node(node, size, extra) 
    struct node* node;
{
    struct rc_node* rc 
        = node ? (unsigned char*)node - sizeof(struct rc_node) : 0;

    /* This is the size of the node before the ops[] payload. */
    int overhead = sizeof(struct node) - sizeof(struct node *[4]);

    if ( !rc || size + extra + overhead > rc->capacity ) {
        size += (extra <= size ? size : extra);
        return rc_realloc( node, size + overhead );
    }

    return node;
}

/* Append node CHILD to node VEC, growing the vector if necessary, 
 * and returning the (possibly reallocated) vector. */
struct node *
vnode_app( vec, child )
    struct node *vec, *child;
{
    vec = grow_node( vec, vec->arity * sizeof(struct node*), 
                     sizeof(struct node*) );
    vec->ops[ vec->arity++ ] = child;
    return vec;
}

/* Returns a pointer to the string payload of a node */
node_str(node) 
    struct node* node;
{
    return (char*) node->ops;
}

/* Returns the node type.  This only exists to abstract the difference
 * between t[0] (in stage-4) and t->code (in stage-5).  */
node_code(node) 
    struct node* node;
{
    return node->code;
}

set_arity(node, arity)
    struct node* node;
{
    node->arity = arity;
}

/* Allocate a string node and set its payload to STR */
struct node*
new_strnode(str)
    char* str;
{
    int sz = strlen(str) + 1;
    struct node* node = grow_node( 0, 0, sz );
    node->code = 'str';
    node->arity = 0;
    node->type = 0;
    strcpy( node_str(node), str, sz );
    return node;
}


/* Append character CHR to the payload of the node *NODE_PTR which is treated 
 * as a string with current length *LEN_PTR.  The value of *LEN_PTR is 
 * incremented.  The node may be reallocated. */
node_lchar( node_ptr, len_ptr, chr )
    struct node** node_ptr;
    int *len_ptr;
{
    struct node* node = grow_node( *node_ptr, *len_ptr, 1 );
    char* buf = node_str(node);
    buf[ (*len_ptr)++ ] = chr;
    *node_ptr = node;
}

/* Push-back facility doesn't really belong here, but having to keep 
 * compatibility with the stage-4 cc is tricky. */
static struct pb_slot {
    struct node* node;
    struct pb_slot* next;
} *pb_stack = 0; 

struct node*
pb_pop() {
    struct node* ret = 0;
    if ( pb_stack ) {
        struct pb_slot* old = pb_stack;
        ret = pb_stack->node;
        pb_stack = pb_stack->next;
        free(old);
    }
    return ret;
}

pb_push(token) 
    struct node* token;
{
    struct pb_slot* p = malloc( sizeof(struct pb_slot*) );
    p->next = pb_stack;
    p->node = token;
    pb_stack = p;
}

node_streq(node, str)
    struct node* node;
    char* str;
{
    return node->code == 'str' && strcmp( node_str(node), str ) == 0;
}


