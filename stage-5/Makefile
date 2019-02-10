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

LD    = $(BUILD)/ld
AS    = $(BUILD)/as
LIBC  = $(BUILD)/libc.o
CRT0  = $(BUILD)/crt0.o

all:	$(BUILD)/ccx $(BUILD)/cpp $(BUILD)/cc $(BUILD)/cmp

INIT  = $(AS) $(LD) $(LIBC) $(CRT0) $(BUILD)/bc

init:	$(INIT) 

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

%0.s:	%.c $(BUILD)/bc
	$(BUILD)/bc -S -o $@ $<

# The --compatibility=4 flag enables compatibility with the stage-4 bc.
%1.s:	%.c $(BUILD)/ccx0
	$(BUILD)/ccx0 --compatibility=4 -o $@ $<

# We cannot remove --compatibility=4 on this build, because of the untyped
# use of structs-as-arrays in the code.  They should be coded as structs,
# but that's not possible because it would be too difficult to add struct 
# support to the stage-4 cc.
%2.s:	%.i $(BUILD)/ccx
	$(BUILD)/ccx --compatibility=4 -o $@ $<

%.i:	%.c $(BUILD)/cpp
	$(BUILD)/cpp -Iinclude -o $@ $<

%.s:	%.c $(BUILD)/ccx
	$(BUILD)/ccx -o $@ $<

# All these files get compiled with --compatibility=4
CCX_OBJS  = scanbase.o scanner.o symtab.o expr.o stmt.o type.o \
            codegen.o i386.o main.o cli.o

# We replace node.o with a new version written using structs (and so that
# won't compile using stage-4 bc), partly as a test of the stage-4 bc.
CCX0_OBJS = $(CCX_OBJS) node.o
CCX1_OBJS = $(CCX_OBJS) nodenew.o

$(BUILD)/ccx0:	$(LD) $(LIBC) $(CRT0) $(CCX0_OBJS:%.o=%0.o)
	$(LD) -o $(BUILD)/$@ $(CRT0) $(CCX0_OBJS:%.o=%0.o) $(LIBC)

$(BUILD)/ccx:	$(LD) $(LIBC) $(CRT0) $(CCX1_OBJS:%.o=%1.o)
	$(LD) -o $(BUILD)/$@ $(CRT0) $(CCX1_OBJS:%.o=%1.o) $(LIBC)

# These files need to compile without preprocessing, and are not given
# --compatiibility=4.  Note that cli.o, scanbase.o and expr.o are also 
# used by the compiler and need also to compile with the stage 4 cc.
CPP_OBJS = scanbase.o cpp.o nodenew.o macros.o pvector.o cli.o expr.o \
           cpptype.o eval.o

$(BUILD)/cpp:	$(LD) $(LIBC) $(CRT0) $(CPP_OBJS)
	$(LD) -o $@ $(CRT0) $(CPP_OBJS) $(LIBC)

CC_OBJS = pvector.o timeconv.o cc.o cli.o

$(BUILD)/cc:	$(LD) $(LIBC) $(CRT0) $(CC_OBJS:%.o=%2.o)
	$(LD) -o $@ $(CRT0) $(CC_OBJS:%.o=%2.o) $(LIBC)

$(BUILD)/cmp:	$(BUILD)/cc cmp.c cli.c
	cd $(BUILD) && ./cc -o $@ ../stage-5/cmp.c ../stage-5/cli.c

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
	$(BUILD)/cmp $(BUILD)/ccx2 $(BUILD)/ccx
	$(BUILD)/cmp $(BUILD)/cc2 $(BUILD)/cc
	! $(BUILD)/cmp -s $(BUILD)/cc2 $(BUILD)/ccx2
	$(RM) -f $(BUILD)/cc2 $(BUILD)/ccx2

# Build ccx2 and cc2 with a single command.  This is a good test of the 
# driver logic.
$(BUILD)/ccx2:	$(BUILD)/cc $(CCX1_OBJS:%.o=%.c)
	cd $(BUILD) && ./cc -o ccx2 --compatibility=4 $(addprefix ../stage-5/,$(CCX1_OBJS:%.o=%.c))

$(BUILD)/cc2:	$(BUILD)/cc $(BUILD)/ccx2 $(CC_OBJS:%.o=%.c)
	cd $(BUILD) && ./cc -o cc2 --with-ccx=ccx2 -I ../stage-5 $(addprefix ../stage-5/,$(CC_OBJS:%.o=%.c))

check:	check-cmp
	$(MAKE) -r -C cpp-tests $@

