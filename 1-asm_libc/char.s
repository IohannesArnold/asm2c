# char.s  --  functions write/read a character to/from a string
# Copyright (C) 2013-2019 Richard Smith <richard@ex-parrot.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

# The functions here are needed to work around the lack of a character type
# in the B language.

.text

####	#  Function:  char rchar(char const* s, size_t n);
	#
	#  B's char function, renamed to avoid forward compatibility problems 
	#  with C's keyword.  Returns the byte S[N], zero padded in a word.
.globl rchar
rchar:
	PUSH	%ebp
	MOVL	%esp, %ebp

	XORL	%eax, %eax
	MOVL	8(%ebp), %edx
	ADDL	12(%ebp), %edx
	MOVB	(%edx), %al

	POP	%ebp
	RET

####	#  Function: char lchar(char* s, size_t n, char c);
	#
	#  B's lchar function.  Sets S[N] = C, and returns C, zero padded in 
	#  a word.
.globl lchar
lchar:
	PUSH	%ebp
	MOVL	%esp, %ebp

	XORL	%eax, %eax
	MOVB	16(%ebp), %al
	MOVL	8(%ebp), %edx
	ADDL	12(%ebp), %edx
	MOVB	%al, (%edx)

	POP	%ebp
	RET
	
