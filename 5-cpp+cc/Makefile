# 5-cpp+cc/Makefile
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

all:	$(BUILD)/bin/cpp #$(BUILD)/bin/cc 

INIT  = $(LIBC) $(CRT0) $(BUILD)/bin/ccx $(BUILD)/include

$(INIT):
	$(MAKE) -C ../4-c_compiler

# Suppress the default rules
.SUFFIXES:

%.o:	%.s
	$(AS) $<

%.s:	%.b $(BUILD)/bin/ccx
	$(BUILD)/bin/ccx -o $@ $<

%.s:	%.c $(BUILD)/bin/ccx
	$(BUILD)/bin/ccx -o $@ $<

%.i:	%.c $(BUILD)/bin/cpp
	$(BUILD)/bin/cpp -I$(BUILD)/include \
	-DINSTALL_DIR="\"$(INSTALL_DIR)\"" -o $@ $<

# These files need to compile without preprocessing. Note that cli.o, 
# scanbase.o and expr.o are also part of the stage 4 compiler and so
# also need to compile with the stage 2 bc.
CPP_OBJS = $(addprefix ../4-c_compiler/,cli.o expr.o scanbase.o nodenew.o) \
	   macros.o pvector.o cpp.o cpptype.o eval.o

$(BUILD)/bin/cpp:	$(LIBC) $(CRT0) $(CPP_OBJS)
	$(LD) -o $@ $^

CC_OBJS = pvector.o timeconv.o cc.o 

#$(BUILD)/bin/cc:	$(LD) $(LIBC) $(CRT0) $(CC_OBJS:%.o=%2.o) ../4-c_compiler/cli.o
#	$(LD) -o $@ $^

.INTERMEDIATE:  $(CPP_OBJS) $(CC_OBJS)

clean:
	$(RM) -f $(CPP_OBJS:%.o=%.s) $(CPP_OBJS) $(BUILD)/cpp
