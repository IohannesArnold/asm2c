/* <fcntl.h>  --  standard C library header 
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

#define O_RDONLY          0
#define O_WRONLY          1
#define O_RDWR            2
#define O_CREAT        0x40
#define O_EXCL         0x80
#define O_TRUNC       0x200
#define O_APPEND      0x400
#define O_DIRECTORY 0x10000
