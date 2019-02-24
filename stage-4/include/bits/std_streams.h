/* <bits/std_streams.h>  --  define stdin, stdout & stderr
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

#ifndef __RBC_BITS_STD_STREAMS_INCLUDED
#define __RBC_BITS_STD_STREAMS_INCLUDED

#include <bits/file.h>

/* C90 7.9.1  The definition of the standard stream macros.
 *
 * They "are expressions of type "pointed to FILE" that point to the FILE 
 * objects associated, respectively, with standard error, input, and output 
 * stream."  But we want ELF objects of the same name. */
extern FILE *stdin, *stdout, *stderr;

#define stdin  stdin
#define stdout stdout
#define stderr stderr

#endif 
