/* pvector.c  --  code to deal with vectors of pointers
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

/* We would like to #include "pvector.h" here, but we don't so that it can
 * be used in the implementation of the preprocessor.  Instead, repeat the
 * definition here. */
struct pvector {
    char **start, **end, **end_store;
};

void *malloc (int size);

struct pvector*
pvec_new() {
    struct pvector* v = malloc( sizeof(struct pvector) );
    int cap = 8;
    v->start = v->end = malloc( sizeof(char*) * cap );
    *v->end = 0;  /* null termination */
    v->end_store = v->start + cap;
    return v;
}

pvec_delete(struct pvector* v) 
{
    if (v) {
        free( v->start );
        free( v );
    }
}

pvec_push(struct pvector* v, char* elt)
{
    /* Overwrite the null termination: which means we're guaranteed to
     * have space at this point. */
    *v->end++ = elt;

    if (v->end == v->end_store) {
        /* We need to reallocate now to push the null terminator */
        int cap = v->end - v->start;
        v->start = (char**) realloc( v->start, sizeof(char*) * 2*cap );
        v->end = v->start + cap;
        v->end_store = v->start + 2*cap;
    }

    *v->end = 0;
}

char* 
pvec_pop(struct pvector* v) 
{
    char* last = *--v->end;
    *v->end = 0;
    return last;
}
