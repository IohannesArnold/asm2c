# unistd.s

# Copyright (C) 2013 Richard Smith <richard@ex-parrot.com>
# All rights reserved.

.data
####	#  Variable:	int errno;
.globl errno
errno:
	.int	0


.text

####	#  Function:	ssize_t write(int fd, const void *buf, size_t count);
	#
.globl write
write:
	PUSH	%ebp
	MOVL	%esp, %ebp
	PUSH	%ebx

	MOVL	16(%ebp), %edx
	MOVL	12(%ebp), %ecx
	MOVL	8(%ebp), %ebx
	MOVL	$4, %eax		# 4 == __NR_write
	INT	$0x80
	CMPL	$-4096, %eax		# -4095 <= %eax < 0 for errno
	JNA	.L1

	NEGL	%eax
	MOVL	%eax, errno
	XORL	%eax, %eax
	DECL	%eax
.L1:
	POP	%ebx
	POP	%ebp
	RET


####	#  Function:	ssize_t read(int fd, void *buf, size_t count);
	#
.globl read
read:
	PUSH	%ebp
	MOVL	%esp, %ebp
	PUSH	%ebx

	MOVL	16(%ebp), %edx
	MOVL	12(%ebp), %ecx
	MOVL	8(%ebp), %ebx
	MOVL	$3, %eax		# 3 == __NR_read
	INT	$0x80
	CMPL	$-4096, %eax		# -4095 <= %eax < 0 for errno
	JNA	.L2

	NEGL	%eax
	MOVL	%eax, errno
	XORL	%eax, %eax
	DECL	%eax
.L2:
	POP	%ebx
	POP	%ebp
	RET
	

####	#  Function:	void _exit(int status)
	#
	#  Terminate program execution with given status.
_exit:
	PUSH	%ebp
	MOVL	%esp, %ebp
	MOVL	8(%ebp), %ebx	
	MOVL	$1, %eax		# 1 == __NR_exit
	INT	$0x80
	HLT


####	#  Function:	int open(char const* filename, int flags, int mode);
	#
.globl open
open:
	PUSH	%ebp
	MOVL	%esp, %ebp
	PUSH	%ebx

	MOVL	16(%ebp), %edx
	MOVL	12(%ebp), %ecx
	MOVL	8(%ebp), %ebx
	MOVL	$5, %eax		# 5 == __NR_open
	INT	$0x80
	CMPL	$-4096, %eax		# -4095 <= %eax < 0 for errno
	JNA	.L3

	NEGL	%eax
	MOVL	%eax, errno
	XORL	%eax, %eax
	DECL	%eax
.L3:
	POP	%ebx
	POP	%ebp
	RET


####	#  Function:	int close(int fd);
	#
.globl close
close:
	PUSH	%ebp
	MOVL	%esp, %ebp
	PUSH	%ebx

	MOVL	8(%ebp), %ebx
	MOVL	$6, %eax		# 6 == __NR_close
	INT	$0x80
	CMPL	$-4096, %eax		# -4095 <= %eax < 0 for errno
	JNA	.L4

	NEGL	%eax
	MOVL	%eax, errno
	XORL	%eax, %eax
	DECL	%eax
.L4:
	POP	%ebx
	POP	%ebp
	RET


####	#  Function:	int dup2(int oldfd, int newfd);
	#
.globl dup2
dup2:
	PUSH	%ebp
	MOVL	%esp, %ebp
	PUSH	%ebx

	MOVL	12(%ebp), %ecx
	MOVL	8(%ebp), %ebx
	MOVL	$63, %eax		# 63 == __NR_dup2
	INT	$0x80
	CMPL	$-4096, %eax		# -4095 <= %eax < 0 for errno
	JNA	.L5

	NEGL	%eax
	MOVL	%eax, errno
	XORL	%eax, %eax
	DECL	%eax
.L5:
	POP	%ebx
	POP	%ebp
	RET


