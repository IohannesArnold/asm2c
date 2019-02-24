/* eval.c  --  evaluate integral constant expressions 
 * Copyright (C) 2014-2019 Richard Smith <richard@ex-parrot.com>
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

/* If only we had a preprocessor to avoid these duplications ... :-) */
struct node {
    int code;           /* character code for the node, e.g. '+' or 'if'. */
    int arity;          /* the number of nodes in the ops[] array. */
    struct node* type;  /* Always NULL in the preprocessor. */
    struct node* ops[4];
};

struct node *add_ref();

static
struct node *
new_num(val) {
    struct node *n = new_node('num', 0);
    n->type = add_ref( implct_int() );
    n->ops[0] = (struct node*) val;
    return n;
}

struct node *
eval(n) 
    struct node *n;
{
    int c = n->code, a = n->arity;
    /* The following are not supported: 
     *   id str unary-* unary-& ++ -- size () [] -> . (cast) , = @=
     */

    if ( a == 0 ) {
        if ( c == 'num' ) return add_ref(n);
        else if ( c == 'chr' ) return new_num( parse_chr( node_str(n) ) );
        else int_error("Unable to evaluate literal '%Mc'", c);
    }
    
    else if ( a == 1 ) {
        struct node *arg = eval( n->ops[0] ), *val;
        if ( c == '+' ) return arg;
        else if ( c == '-' ) val = new_num( -node_ival(arg) );
        else if ( c == '~' ) val = new_num( ~node_ival(arg) );
        else if ( c == '!' ) val = new_num( !node_ival(arg) );
        else int_error("Unable to evaluate unary '%Mc'", c);
        free_node(arg);
        return val;
    }

    else if ( a == 2 ) {
        struct node *lhs = eval( n->ops[0] ), *rhs = eval( n->ops[1] ), *val;
        if ( c == ',' ) val = add_ref(rhs);
        else if ( c == '*'  ) val = new_num( node_ival(lhs) *  node_ival(rhs) );
        else if ( c == '/'  ) val = new_num( node_ival(lhs) /  node_ival(rhs) );
        else if ( c == '%'  ) val = new_num( node_ival(lhs) %  node_ival(rhs) );
        else if ( c == '+'  ) val = new_num( node_ival(lhs) +  node_ival(rhs) );
        else if ( c == '-'  ) val = new_num( node_ival(lhs) -  node_ival(rhs) );
        else if ( c == '<<' ) val = new_num( node_ival(lhs) << node_ival(rhs) );
        else if ( c == '>>' ) val = new_num( node_ival(lhs) >> node_ival(rhs) );
        else if ( c == '<'  ) val = new_num( node_ival(lhs) <  node_ival(rhs) );
        else if ( c == '>'  ) val = new_num( node_ival(lhs) >  node_ival(rhs) );
        else if ( c == '<=' ) val = new_num( node_ival(lhs) <= node_ival(rhs) );
        else if ( c == '>=' ) val = new_num( node_ival(lhs) >= node_ival(rhs) );
        else if ( c == '==' ) val = new_num( node_ival(lhs) == node_ival(rhs) );
        else if ( c == '!=' ) val = new_num( node_ival(lhs) != node_ival(rhs) );
        else if ( c == '&'  ) val = new_num( node_ival(lhs) &  node_ival(rhs) );
        else if ( c == '^'  ) val = new_num( node_ival(lhs) ^  node_ival(rhs) );
        else if ( c == '|'  ) val = new_num( node_ival(lhs) |  node_ival(rhs) );
        /* We don't care about whether these short circuit as it is not 
         * possible to detect that. */
        else if ( c == '&&' ) val = new_num( node_ival(lhs) && node_ival(rhs) );
        else if ( c == '||' ) val = new_num( node_ival(lhs) || node_ival(rhs) );
        else int_error("Unable to evaluate binry '%Mc'", c);
        free_node(rhs); free_node(lhs);
        return val;
    }

    else if ( c == '?:' ) {
        struct node *cond = eval( n->ops[0] );
        int val = node_ival(cond);
        free_node(cond);
        return eval( n->ops[ val ? 1 : 2 ] );
    }

    else int_error("Unable to evaluate unknown operator '%Mc'", c);
}
