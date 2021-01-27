/* stdarg.b
 * Copyright (C) 2005-2019 Richard Smith <richard@ex-parrot.com>
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

/* The intention is that the C library macro va_arg can be implemented as:
 * 
 *   #define va_arg( ap, type ) \
 *     ( * (type*) __va_arg( &(ap), sizeof(type) ) )
 */
__va_arg( ap, size ) {
    auto a = *ap;
    /* Round size to 4 byte alignment */
    size = (size + 3) & ~3;
    *ap += size;
    return a;
}

/* The intention is that the C library macro va_start can be implemented as:
 *
 *   #define va_start( ap, last ) \
 *     ( __va_start( &(ap), &(last), sizeof(last) ) )                     
 */
__va_start( ap, last, last_size ) {
    *ap = last;
    __va_arg( ap, last_size );
}
