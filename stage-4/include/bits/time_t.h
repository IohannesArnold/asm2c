/* <bits/struct_tm.h>  --  the definition of struct tm 
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

#ifndef __RBC_BITS_TIME_T_INCLUDED
#define __RBC_BITS_TIME_T_INCLUDED

/* time_t
 *
 * The C standard simply requires this to be arithmetic, and not necessarily
 * even signed.  The Linux kernel ABI makes it a 32-bit signed type. 
 */
typedef int time_t;

#endif