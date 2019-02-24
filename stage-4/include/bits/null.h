/* <bits/null.h>  --  the NULL macro is defined by multiple headers
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

#ifndef __RBC_BITS_NULL_INCLUDED
#define __RBC_BITS_NULL_INCLUDED

/* C90 7.1.6 Common definitions <stddef.h> (part implementation)
 *
 * "An integral constant expression with the value 0, or such an 
 * expression cast to type void *, is called a null pointer constant."
 * [C90 6.2.2.3]
 *
 * NULL "expands to an implementation-defined null pointer
 * constant." [C90 7.1.6]
 */

/* FIXME  There is no void type in stage 5. */
#define NULL 0

#endif
