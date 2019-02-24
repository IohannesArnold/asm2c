/* <bits/file_access.h>  --  file access functions for <stdio.h>
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

#ifndef __RBC_BITS_FILE_ACCESS_INCLUDED
#define __RBC_BITS_FILE_ACCESS_INCLUDED

#include <bits/file.h>
#include <bits/size_t.h>

/* These macros match values are hardcoded into stage-4/output.c. */

/* Values suitable for the mode argument to setvbuf */
#define _IOFBF  1  /* Fully buffered */
#define _IOLBF  2  /* Line buffered */
#define _IONBF  3  /* Unbuffered */

/* The size buffer required as the buf argument to setbuf. 
 * Note: This is not (necessarily) the default buffer size. */
#define BUFSIZ  4096

#if 0
/* C90 7.9.5.1:  The fclose function */
int fclose( FILE* stream );

/* C90 7.9.5.2:  The fflush function */
int fflush( FILE* stream );

/* C90 7.9.5.3:  The fopen function */
FILE *fopen( char const* filename, char const* mode );

/* C90 7.9.5.4:  The freopen function */
FILE *freopen( char const *filename, char const *mode, FILE *stream ); 

/* C90 7.9.5.5:  The setbuf function */
void setbuf( FILE *stream, char *buf );

/* C90 7.9.5.6:  The setvbuf function */
int setvbuf( FILE *stream, char *buf, int mode, size_t size );

#else

/* Temporary versions while we don't support prototypes. */
FILE *freopen();
FILE *fopen();

#endif

#endif
