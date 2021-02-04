/* cli.b  --  command line interface utils
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

cli_error(fmt) 
    char *fmt;
{
    extern stderr;
    vfprintf(stderr, fmt, &fmt);
    exit(1);
}

char* opt_arg(argv, argc, argnptr, argname)
    char **argv;
    int argc, *argnptr;
    char *argname;
{
    auto char *arg = argv[*argnptr];
    auto int arglen = strlen(argname);
    if ( strncmp( arg, argname, arglen ) == 0 ) {
        if ( rchar( arg, arglen ) == 0 ) {
            if ( ++*argnptr == argc )
                cli_error("The %s option takes an argument\n", argname);
            arg = argv[*argnptr];
            ++*argnptr;
            return arg;
        }
        /* Short arguments (e.g. -X) do not have an '=' before their values. */
        else if ( arglen == 2 ) {
            arg += arglen;
            ++*argnptr;
            return arg;
        }
        /* Long arguments (e.g. --foo) need an '=' before their values. */
        else if ( rchar( arg, arglen ) == '=' ) {
            arg += arglen + 1;
            ++*argnptr;
            return arg;
        }
    }
    return 0;
}


