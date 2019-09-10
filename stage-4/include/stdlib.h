/* <stdlib.h>  --  standard C library header 
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

#ifndef __STDLIB_H__
#define __STDLIB_H__
#include <bits/null.h>
#include <bits/size_t.h>
int abs(int i);

void* malloc(size_t sz);
void free(void* ptr);
void* realloc(void* ptr, size_t sz);

void abort(void);

#endif
