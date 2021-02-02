/* <stdarg.h>  --  standard C library header
 * Copyright (C) 2019 John Arnold
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

#ifndef _STDARG_H
#define _STDARG_H

typedef char *va_list;

/* only correct for i386 */
#define va_start(ap,last) ap=(char *)&(last); 
#define va_arg(ap,type) (ap-=sizeof (type), *(type *)(ap))
#define va_end(ap)

#endif
