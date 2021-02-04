# 4-c_compiler/Makefile
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

all:	$(BUILD)/bin/ccx

INIT  = $(LIBC) $(CRT0) $(BUILD)/bc $(BUILD)/include

$(INIT):
	$(MAKE) -C ../3-b_libc

# bc   is the compiler from stage 2.
# ccx0 is this stage's compiler (written in the B language) compiled with bc.
# ccx  is nearly the same code, though with a replacement node.c, compiled 
#        by itself (by ccx0), and is therefore smaller.
# ccx2 is a test compiler produced by ccx; it should be binary identical to ccx.

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

%.s:	%.b $(BUILD)/bin/ccx
	$(BUILD)/bin/ccx -o $@ $<

%.s:	%.c $(BUILD)/bin/ccx
	$(BUILD)/bin/ccx -o $@ $<

CCX_OBJS  = main.o scanbase.o scanner.o symtab.o expr.o stmt.o type.o \
            codegen.o i386.o

# We replace node.o with a new version written using structs (and which
# therefore won't compile using stage 2 bc), partly as a test of the stage 2 bc.
CCX0_OBJS = $(CCX_OBJS) node.o
CCX1_OBJS = $(CCX_OBJS) nodenew.o

$(BUILD)/ccx0:	$(LIBC) $(CRT0) $(CCX0_OBJS:%.o=%0.o)
	$(LD) -o $@ $^

$(BUILD)/bin/ccx:	$(LIBC) $(CRT0) $(CCX1_OBJS:%.o=%1.o)
	$(LD) -o $@ $^

.INTERMEDIATE:  $(CCX0_OBJS:%.o=%0.o) $(CCX1_OBJS:%.o=%1.o) \
                $(CCX1_OBJS:%.o=%.s) $(CCX1_OBJS)
clean:
	$(RM) -f $(CCX0_OBJS:%.o=%0.s) $(CCX0_OBJS:%.o=%0.o) $(BUILD)/ccx0
	$(RM) -f $(CCX1_OBJS:%.o=%1.s) $(CCX1_OBJS:%.o=%1.o) $(BUILD)/ccx
