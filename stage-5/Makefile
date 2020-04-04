# stage-5/Makefile
# Copyright (C) 2009-2019 Richard Smith <richard@ex-parrot.com>
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

include ../config.make
BUILD = ../build

LD    = $(BUILD)/bin/ld
AS    = $(BUILD)/bin/as
LIBC  = $(BUILD)/lib/libc.o
CRT0  = $(BUILD)/lib/crt0.o

all:	$(BUILD)/bin/ccx $(BUILD)/bin/cpp $(BUILD)/bin/cc 

INIT  = $(AS) $(LD) $(LIBC) $(CRT0) $(BUILD)/bc $(BUILD)/include

# bc   is the compiler from stage 4.
# ccx0 is this stage's compiler (written in the B language) compiled with bc.
# ccx  is nearly the same code, though with a replacement node.c, compiled 
#        by itself (by ccx0), and is therefore smaller.
# ccx2 is a test compiler produced by ccx; it should be binary identical to ccx.

$(INIT):
	$(MAKE) -C ../stage-4

# Suppress the default rules
.SUFFIXES:

%.o:	%.s $(AS)
	$(AS) $<

%0.s:	%.b $(BUILD)/bc
	$(BUILD)/bc -S -o $@ $<

%1.s:	%.b $(BUILD)/ccx0
	$(BUILD)/ccx0 -o $@ $<

%1.s:	%.c $(BUILD)/ccx0
	$(BUILD)/ccx0 -o $@ $<

%2.s:	%.i $(BUILD)/bin/ccx
	$(BUILD)/bin/ccx -o $@ $<

%.i:	%.c $(BUILD)/bin/cpp
	$(BUILD)/bin/cpp -I$(BUILD)/include -DINSTALL_DIR="\"$(INSTALL_DIR)\"" -o $@ $<

%.s:	%.b $(BUILD)/bin/ccx
	$(BUILD)/bin/ccx -o $@ $<

%.s:	%.c $(BUILD)/bin/ccx
	$(BUILD)/bin/ccx -o $@ $<

CCX_OBJS  = scanbase.o scanner.o symtab.o expr.o stmt.o type.o \
            codegen.o i386.o ccx.o cli.o

# We replace node.o with a new version written using structs (and so that
# won't compile using stage-4 bc), partly as a test of the stage-4 bc.
CCX0_OBJS = $(CCX_OBJS) node.o
CCX1_OBJS = $(CCX_OBJS) nodenew.o

$(BUILD)/ccx0:	$(LD) $(LIBC) $(CRT0) $(CCX0_OBJS:%.o=%0.o)
	$(LD) -o $@ $(CRT0) $(CCX0_OBJS:%.o=%0.o) $(LIBC)

$(BUILD)/bin/ccx:	$(LD) $(LIBC) $(CRT0) $(CCX1_OBJS:%.o=%1.o)
	$(LD) -o $@ $(CRT0) $(LIBC) $(CCX1_OBJS:%.o=%1.o) 

# These files need to compile without preprocessing. Note that cli.o, 
# scanbase.o and expr.o are also used by the compiler and need also 
# to compile with the stage 4 bc.
CPP_OBJS = scanbase.o cpp.o nodenew.o macros.o pvector.o cli.o expr.o \
           cpptype.o eval.o

$(BUILD)/bin/cpp:	$(LD) $(LIBC) $(CRT0) $(CPP_OBJS)
	$(LD) -o $@ $(CRT0) $(CPP_OBJS) $(LIBC)

CC_OBJS = pvector.o timeconv.o cc.o 

$(BUILD)/bin/cc:	$(LD) $(LIBC) $(CRT0) $(CC_OBJS:%.o=%2.o) cli.o
	$(LD) -o $@ $(CRT0) $(LIBC) $(CC_OBJS:%.o=%2.o) cli.o 

$(BUILD)/cmp:	$(BUILD)/bin/cc cmp.c cli.b
	$(BUILD)/bin/cc --with-cpp=$(BUILD)/bin/cpp --with-ccx=$(BUILD)/bin/ccx --with-as=$(BUILD)/bin/as --with-ld=$(BUILD)/bin/ld -I$(BUILD)/include --nostdlib -o $(BUILD)/cmp $(CRT0) $(LIBC) cmp.c cli.b

.INTERMEDIATE:  $(CCX0_OBJS:%.o=%0.o) $(CCX1_OBJS:%.o=%1.o) \
                $(CCX1_OBJS:%.o=%.s) $(CCX1_OBJS) $(CPP_OBJS) $(CC_OBJS)

clean:
	$(RM) -f $(CCX0_OBJS:%.o=%0.s) $(CCX0_OBJS:%.o=%0.o) $(BUILD)/ccx0
	$(RM) -f $(CCX1_OBJS:%.o=%1.s) $(CCX1_OBJS:%.o=%1.o) $(BUILD)/ccx
	$(RM) -f $(CPP_OBJS:%.o=%.s) $(CPP_OBJS) $(BUILD)/cpp
	$(RM) -f $(CCX1_OBJS:%.o=%2.s) $(CCX1_OBJS:%.o=%2.i) 
	$(RM) -f $(CC_OBJS:%.o=%.i) $(CC_OBJS:%.o=%2.s) $(CC_OBJS:%.o=%2.s) $(BUILD)/cc
	$(RM) -f $(CCX1_OBJS:%.o=%2.o) $(BUILD)/ccx2 $(CC_OBJS:%.o=%2.o) $(BUILD)/cc2
	$(RM) -f cmp.o cmp.s cmp.i $(BUILD)/cmp

check-cmp: $(BUILD)/ccx2 $(BUILD)/cc2 $(BUILD)/cmp
	$(BUILD)/cmp $(BUILD)/ccx2 $(BUILD)/bin/ccx
	$(BUILD)/cmp $(BUILD)/cc2 $(BUILD)/bin/cc
	! $(BUILD)/cmp -s $(BUILD)/cc2 $(BUILD)/ccx2
	$(RM) -f $(BUILD)/cc2 $(BUILD)/ccx2

# Build ccx2 and cc2 with a single command.  This is a good test of the 
# driver logic.
$(BUILD)/ccx2:	$(BUILD)/bin/cc $(CCX_OBJS:%.o=%.b) nodenew.c
	$(BUILD)/bin/cc --with-cpp=$(BUILD)/bin/cpp --with-ccx=$(BUILD)/bin/ccx --with-as=$(BUILD)/bin/as --with-ld=$(BUILD)/bin/ld -I$(BUILD)/include --nostdlib -o $(BUILD)/ccx2 $(CRT0) $(LIBC) $(CCX_OBJS:%.o=%.b) nodenew.c

$(BUILD)/cc2:	$(BUILD)/bin/cc $(BUILD)/ccx2 $(CC_OBJS:%.o=%.c) cli.b
	$(BUILD)/bin/cc --with-cpp=$(BUILD)/bin/cpp --with-ccx=$(BUILD)/ccx2 --with-as=$(BUILD)/bin/as --with-ld=$(BUILD)/bin/ld -I$(BUILD)/include -I./ --nostdlib -o $(BUILD)/cc2 $(CRT0) $(LIBC) $(CC_OBJS:%.o=%.c) cli.b

check:	check-cmp
#	Commented out while the cpp tests don't work
#	$(MAKE) -r -C cpp-tests $@

