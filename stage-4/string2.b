/* string2.b  --  additional, higher-level string handling functions
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

/* The BSD extension strlcat() */
strlcat(dest, src, n) {
    auto l1 = strnlen(dest, n), l2 = strnlen(src, n-l1);
    strncpy( dest + l1, src, l2 );
    lchar(dest, l1 + l2 < n ? l1 + l2 : n - 1, '\0');
    return dest;
}   

/* The C library strcat() */
strcat(dest, src) {
    auto l1 = strlen(dest), l2 = strlen(src);
    strcpy( dest + l1, src );
    lchar(dest, l1 + l2, '\0');
    return dest;
}

/* The C library memmove() */
memmove(dest, src, n) {
    /* If we're copying to earlier memory, or if the blocks do not overlap,
     * then a forwards copy, as done by memcpy, will be fine. */
    if ( dest < src || dest > src + n ) return memcpy(dest, src, n);
 
    /* Otherwise we set the direction flag (DF), then call memcpy with the
     * end pointers (which will then copy backwards), and clear DF. 
     * We do not clear DF in memcpy because the ABI requires DF always to
     * be cleared before library calls. */
    __asm_std(); 
    memcpy(dest+n-1, src+n-1, n); 
    __asm_cld();
    return dest;
}

/* The C library strdup() */
strdup( str ) {
    auto l = strlen(str);
    auto str2 = malloc(l + 1);
    strcpy( str2, str );
    lchar( str2, l, 0 );
    return str2;
}
