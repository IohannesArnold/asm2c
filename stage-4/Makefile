# stage-4/Makefile

# Copyright (C) 2012, 2013, 2014, 2016 Richard Smith <richard@ex-parrot.com>
# All rights reserved.

SHELL = /bin/sh
PATH  = .

RM    = /bin/rm
MAKE  = /usr/bin/make
CMP   = /usr/bin/cmp
AS    = ../bin/as3
LD    = ../bin/ld3

all:	cc

init:	../bin/as3 ../bin/ld3

../bin/as3 ../bin/ld3:
	$(MAKE) -C ../stage-3

# Suppress the default rules
.SUFFIXES:

%.o:	%.s ../bin/as3
	$(AS) $<

%.s:	%.c cc0
	../bin/cc4_0 -S $<

# LIB_OBJS contain a minimal C library written entirely in assembler.
# LIB0_OBJS adds bootstrap .o files written in assembler that will be replaced.
# LIB1_OBJS adds to and replaces the LIB0_OBJS with versions written in C.

LIB_OBJS = string.o ctype.o unistd.o char.o imath.o
LIB0_OBJS = $(LIB_OBJS) error.o stdio.o memory.o
LIB1_OBJS = $(LIB_OBJS) exit.o output.o input.o malloc.o signal.o string2.o \
  stdarg.o

CC_OBJS  = i386.o scanner.o symtab.o expr.o stmt.o main.o

# libc0.o is the primative libc, written solely in assembler.
# cc0     is the compiler linked against the libc0.o.
# libc.o  is the complete stage-4 libc, with some C code compiled using cc0.
# cc      is the compiler relinked against the new libc.o.

libc0.o:	$(LD) $(LIB0_OBJS)
	$(LD) -r -o ../bin/libc0.o $(LIB0_OBJS)

cc0:	$(LD) libc0.o crt0.o $(CC_OBJS)
	$(LD) -o ../bin/cc4_0 ../bin/libc0.o crt0.o $(CC_OBJS)


libc.o:	$(LD) $(LIB1_OBJS)
	$(LD) -r -o ../bin/libc.o $(LIB1_OBJS)

cc:	$(LD) libc.o crt0.o $(CC_OBJS)
	$(LD) -o ../bin/cc4 ../bin/libc.o crt0.o $(CC_OBJS)

.INTERMEDIATE:	$(CC_OBJS) $(LIB1_OBJS) $(LIB0_OBJS) libc0.o cc0

check:	check-output check-input check-malloc

check-output: cc as output.c output.o
	$(LN_S) output.c output2.c
	cc -S output2.c
	as output2.s
	$(CMP) output2.o output.o
	$(RM) output2.c output2.o output2.s

check-input: cc as input.c input.o
	$(LN_S) input.c input2.c
	cc -S input2.c
	as input2.s
	$(CMP) input2.o input.o
	$(RM) input2.c input2.o input2.s

check-malloc: cc as malloc.c malloc.o
	$(LN_S) malloc.c malloc2.c
	cc -S malloc2.c
	as malloc2.s
	$(CMP) malloc2.o malloc.o
	$(RM) malloc2.c malloc2.o malloc2.s

clean:
	$(RM) -f as ld $(LIB0_OBJS)
	$(RM) -f $(LIB1_OBJS) output.s input.s malloc.s signal.s libc.o
	$(RM) -f $(CC_OBJS) cc0 cc crt0.o
	$(RM) -f output2.o output2.s input2.o input2.s malloc2.o malloc2.s

world:
	$(MAKE) clean
	$(MAKE) all
	$(MAKE) check
