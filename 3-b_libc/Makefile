# 3-b_libc/Makefile
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
BUILD = ../$(BUILD_DIR)

LD = $(BUILD)/bin/ld
AS = $(BUILD)/bin/as

all:	$(BUILD)/lib/libc.o $(BUILD)/include

$(BUILD)/bc:
	$(MAKE) -C ../2-b_compiler

# Suppress the default rules
.SUFFIXES:

%.o:	%.s 
	$(AS) $<

%.s:	%.b $(BUILD)/bc
	$(BUILD)/bc -S $<

# LIB1_OBJS contains the assembly files we still need
# LIB_OBJS adds to and replaces other assembly file with versions written in B.

LIB1_OBJS = $(addprefix ../1-asm_libc/,string.o ctype.o unistd.o char.o imath.o)
LIB_OBJS = $(LIB1_OBJS) exit.o output.o input.o malloc.o signal.o string2.o \
	    stdarg.o unistd2.o

$(BUILD)/lib/libc.o:	$(LIB_OBJS)
	$(LD) -r -o $@ $^

$(BUILD)/include:	./include
	$(CP) -r ./include $(BUILD)

.INTERMEDIATE:	$(LIB_OBJS)

