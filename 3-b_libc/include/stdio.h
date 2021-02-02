/* <stdio.h>  --  standard C library header for input/output
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

#ifndef __STDIO_H__
#define __STDIO_H__

#include <bits/null.h>
#include <bits/size_t.h>

#define EOF (-1)

typedef struct __stdio_file_t FILE;

extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

#define stdin stdin
#define stdout stdout
#define stderr stderr

#define _IOFBF  1  /* Fully buffered */
#define _IOLBF  2  /* Line buffered */
#define _IONBF  3  /* Unbuffered */
#define BUFSIZ  4096

int fclose(FILE *stream);
int fflush(FILE *stream);
FILE *fopen(char const *filename, char const *mode);
FILE *fdopen(int fd, const char *mode);
FILE *freopen(char const *filename, char const *mode, FILE *stream);
void setbuf(FILE *stream, char *buf);
int setvbuf(FILE *stream, char *buf, int mode, size_t size);
size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
int fputs(const char *str, FILE *stream);
int puts(const char *str);
int fputc(int c, FILE *stream);
int putc(int c, FILE *stream);
int putchar(int c);

#endif
