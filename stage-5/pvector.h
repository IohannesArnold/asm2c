/* pvector.h  --  code to deal with vectors of pointers
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

#ifndef RBC5_PVECTOR_INCLUDED
#define RBC5_PVECTOR_INCLUDED

struct pvector {
    char **start, **end, **end_store;
};

struct pvector* pvec_new();
pvec_delete();
pvec_push();
char* pvec_pop();

#endif
