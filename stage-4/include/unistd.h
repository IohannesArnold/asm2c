/* <unistd.h>  --  standard C library header 
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

#ifndef __UNISTD_H__
#define __UNISTD_H__
#include <bits/size_t.h>
typedef signed int ssize_t

ssize_t write(int fd, const void *buf, size_t count);
ssize_t read(int fd, void *buf, size_t count);
int open(char const* filename, int flags, int mode);
int close(int fd);
int dup2(int oldfd, int newfd);
int execve(char* filename, char* argv[], char* envp[]);
int fork();
int unlink(char* filename);
#endif
