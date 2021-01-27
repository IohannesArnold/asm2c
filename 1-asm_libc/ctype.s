# ctype.s
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

.data 
.local ctype_tbl
ctype_tbl:

#  This table contains character bits for the 0x80 characters in ASCII:
#
#    space = 0x1,  print = 0x2,  cntrl = 0x4,  upper = 0x8, 
#    lower = 0x10, alpha = 0x20, digit = 0x40, punct = 0x80, xdigit = 0x100
#
#  The categories match those in [category.ctype] in C++ and enable
#  effecient implementation of the C standard library ctype functions.
#  NB. We would ideally use .short, but the level 3 as doesn't support 
#  either the directive or the necessary operations on 16-bit registers.

.int	0x004,  0x004,  0x004,  0x004,  0x004,  0x004,  0x004,  0x004	# 0x00
.int	0x004,  0x005,  0x005,  0x005,  0x005,  0x005,  0x004,  0x004	
.int	0x004,  0x004,  0x004,  0x004,  0x004,  0x004,  0x004,  0x004	# 0x10
.int	0x004,  0x004,  0x004,  0x004,  0x004,  0x004,  0x004,  0x004
.int	0x003,  0x082,  0x082,  0x082,  0x082,  0x082,  0x082,  0x082	# 0x20
.int	0x082,  0x082,  0x082,  0x082,  0x082,  0x082,  0x082,  0x082
.int	0x142,  0x142,  0x142,  0x142,  0x142,  0x142,  0x142,  0x142	# 0x30
.int	0x142,  0x142,  0x082,  0x082,  0x082,  0x082,  0x082,  0x082
.int	0x082,  0x12A,  0x12A,  0x12A,  0x12A,  0x12A,  0x12A,  0x02A	# 0x40
.int	0x02A,  0x02A,  0x02A,  0x02A,  0x02A,  0x02A,  0x02A,  0x02A
.int	0x02A,  0x02A,  0x02A,  0x02A,  0x02A,  0x02A,  0x02A,  0x02A	# 0x50
.int	0x02A,  0x02A,  0x02A,  0x082,  0x082,  0x082,  0x082,  0x082
.int	0x082,  0x132,  0x132,  0x132,  0x132,  0x132,  0x132,  0x032	# 0x60
.int	0x032,  0x032,  0x032,  0x032,  0x032,  0x032,  0x032,  0x032
.int	0x032,  0x032,  0x032,  0x032,  0x032,  0x032,  0x032,  0x032	# 0x70
.int	0x032,  0x032,  0x032,  0x082,  0x082,  0x082,  0x082,  0x004


.text
.local getctype
####	#  Function:	int getctype(int chr);
	#
	#  A utility function to return the character class(es) of CHR.
getctype:
	PUSH	%ebp
	MOVL	%esp, %ebp

	#  Characters >= 0x80 (as unsigned, which catches EOF) return 0
	XORL	%eax, %eax
	MOVL	8(%ebp), %edx
	CMPL	$0x80, %edx	
	JAE	.L1

	#  Find ctype_tbl[%eax]
	#  Unfortunately, stage 3 as doesn't support MOVL (%eax,%edx,4), %eax.
	MOVB	$2, %cl
	SHLL	%edx			# Multiply %edx by 2
	MOVL	$ctype_tbl, %eax
	ADDL	%edx, %eax
	MOVL	(%eax), %eax

.L1:
	POP	%ebp
	RET



####	#  Function:	int isspace(int c);
	#
	#  Standard C library function to test for ASCII space characters.
.globl isspace
isspace:
	PUSH	%ebp
	MOVL	%esp, %ebp
	PUSH	8(%ebp)
	CALL	getctype
	POP	%ecx
	ANDL	$0x1, %eax
	POP	%ebp
	RET


####	#  Function: int isdigit(int c);
	#
	#  Standard C library function to test for ASCII digits, 0-9.
.globl isdigit
isdigit:
	PUSH	%ebp
	MOVL	%esp, %ebp
	PUSH	8(%ebp)
	CALL	getctype
	POP	%ecx
	ANDL	$0x40, %eax
	POP	%ebp
	RET


####	#  Function:	int isalpha(int c);
	#
	#  Standard C library function to test for ASCII letter, a-z, A-Z.
.globl isalpha
isalpha:
	PUSH	%ebp
	MOVL	%esp, %ebp
	PUSH	8(%ebp)
	CALL	getctype
	POP	%ecx
	ANDL	$0x20, %eax
	POP	%ebp
	RET
	

####	#  Function:	int isalnum(int c);
	#
	#  Standard C library function to test for ASCII letters or digits.
.globl isalnum
isalnum:
	PUSH	%ebp
	MOVL	%esp, %ebp
	PUSH	8(%ebp)
	CALL	getctype
	POP	%ecx
	ANDL	$0x60, %eax
	POP	%ebp
	RET

	
####	#  Function:	int ispunct(int c);
	#
	#  Standard C library function to test for ASCII punctuation,
	#  i.e. anything that is not a digit, letter, space or control.
.globl ispunct
ispunct:
	PUSH	%ebp
	MOVL	%esp, %ebp
	PUSH	8(%ebp)
	CALL	getctype
	POP	%ecx
	ANDL	$0x80, %eax
	POP	%ebp
	RET


####	#  Function: int isxdigit(int c);
	#
	#  Standard C library function to test for hex ASCII digits, 0-9A-Fa-f.
.globl isxdigit
isxdigit:
	PUSH	%ebp
	MOVL	%esp, %ebp
	PUSH	8(%ebp)
	CALL	getctype
	POP	%ecx
	ANDL	$0x100, %eax
	POP	%ebp
	RET


