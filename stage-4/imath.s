# imath.s  --  functions for integer maths
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

.text

####	#  Function:  int abs(int i);
	#
	#  The C library's abs() function.
.globl	abs
abs:
	PUSH	%ebp
	MOVL	%esp, %ebp
	MOVL	8(%ebp), %eax

	#  Implementation without branching
	CDQ				# sign extend into %edx:%eax
	XORL	%edx, %eax
	SUBL	%edx, %eax

	POP	%ebp
	RET


####	#  Function:	unsigned __mul_add( unsigned* val, 
	#                                   unsigned mul, unsigned add );
	# 
	#  Calculate *val = *val * mul + add and return the carry bits
	#  (i.e. the high 32 bits).  This is used in strtoul().
.globl	__mul_add
__mul_add:
	PUSH	%ebp
	MOVL	%esp, %ebp

	MOVL	8(%ebp), %ecx
	MOVL	(%ecx), %eax
	MULL	12(%ebp)		# %edx:%eax <= *val * mul (unsigned)

	XORL	%ecx, %ecx		# do first as it clears CF
	ADDL	16(%ebp), %eax		# %eax += add;  sets CF
	ADCL	%ecx, %edx		# %edx += CF (%ecx is zero)
	
	MOVL	8(%ebp), %ecx
	MOVL	%eax, (%ecx)		# Update *val
	MOVL	%edx, %eax		# Return carry bits from %edx

	POP	%ebp
	RET
