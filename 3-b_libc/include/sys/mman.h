/* <sys/mman.h>  --  standard C library header 
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

#include <bits/size_t.h>

#define	PROT_READ	0x04	/* pages can be read */
#define	PROT_WRITE	0x02	/* pages can be written */
#define	PROT_EXEC	0x01	/* pages can be executed */

#define	MAP_ANONYMOUS	0x0002	/* allocated from memory, swap space */
#define	MAP_PRIVATE	0x0000	/* changes are private */

void *mmap(void *addr, size_t length, int prot, int flags, int fd, size_t offset);
