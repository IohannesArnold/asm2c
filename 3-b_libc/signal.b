/* signal.b  --  signal handling functions
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

/* The C library raise() */
raise( sig ) {
    return kill( getpid(), sig );
}

/* The C library abort() */
abort() {
    raise(6);   /* SIGABRT == 6 */

    /* If we're still here, reinstate the default handler and retry. */
    signal(6, 0);   /* SIG_DFL == 0 */
    raise(6);

    /* This shouldn't be possible. */
    _exit(128 + 6);
}
