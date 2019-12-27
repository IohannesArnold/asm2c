/* rbc_init.h  --  definitions read at the start of every compilation 
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

/* The primary purpose of this file was so that neither the compiler
 * driver, nor the preprocessor, need to be updated when the compiler
 * is updated to include new functionality. */

/* To allow headers in this stage to be used later, it's convenient to
 * allow some extra keywords to appear in them. */
#define const
