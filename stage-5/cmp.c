/* cmp.c  --  an implementation of the POSIX cmp(1) utility
 * Copyright (C) 2015-2019 Richard Smith <richard@ex-parrot.com>
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

#include <stdio.h> 

usage() {
    cli_error("Usage: cmp [-l|-s] file1 file2\n");
}

main(int argc, char **argv) 
{
    char *na, *nb;
    FILE *a, *b;
    int opt_s = 0, opt_l = 0;
    int i = 1, bytes = 0, lines = 1, status = 0;

    if (argc < 3) usage();

    /* The -s option suppressed output. */
    if ( strcmp( argv[i], "-s" ) == 0 )
        opt_s = 1, ++i;
    /* The -l option prints a byte-by-byte comparison in a whacky format. */
    else if ( strcmp( argv[i], "-l" ) == 0 )
        opt_l = 1, ++i;

    na = argv[i++];
    if ( strcmp( na, "-" ) ) {
        a = fopen( na, "r" );
        if (!a) cli_error( "cmp: unable to open file '%s'\n", na );
    }
    else a = stdin;

    nb = argv[i++];
    if ( strcmp( nb, "-" ) ) {
        b = fopen( nb, "r" );
        if (!b) cli_error( "cmp: unable to open file '%s'\n", nb );
    }
    else if ( a == stdin ) 
        cli_error( "cmp: cannot read standard input twice\n" );
    else b = stdin;

    if (i != argc) usage();

    while (1) {
        int ca = fgetc(a), cb = fgetc(b);
        ++bytes;

        /* The format of these messages, and whether they go to stdout or 
         * stderr, is prescribed by POSIX. */
        if ( ca == EOF && cb == EOF ) 
            break;
        else if ( ca == EOF || cb == EOF ) {
            if (!opt_s) {
                fflush(stdout);
                fprintf(stderr, "cmp: EOF on %s\n", ca == EOF ? na : nb);
            }
            status = 1; 
            break;
        }
        else if ( ca != cb ) {
            status = 1;
            if (opt_l)
                printf( "%d %o %o\n", bytes, ca, cb );
            else {
                if (!opt_s) 
                    printf( "%s %s differ: char %d, line %d\n", 
                            na, nb, bytes, lines );
                break;
            }
        }
        else if ( ca == '\n' ) 
            ++lines;
    }

    return status;
}
