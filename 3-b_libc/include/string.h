/* <string.h>  --  standard C library header 
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

#ifndef __STRING_H__
#define __STRING_H__

#include <bits/null.h>
#include <bits/size_t.h>

size_t strlen(char *s);
size_t strnlen(char *s, size_t maxlen);
int strcmp(char const *a, char const *b);
int strncmp(char const *a, char const *b, size_t n);
char *strchr(char const *a, int c);
char *strrchr(char const *a, int c);
char *strcpy(char *dest, char const *str);
char *strncpy(char *dest, char const *str, size_t n);
char *strcat(char *dest, const char *src);
char *strdup(const char *s);

void *memcpy(void* dest, void const *src, size_t n);
void *memset(void* s, int c, size_t n);
void *memmove(void *str1, const void *str2, size_t n);
#endif
