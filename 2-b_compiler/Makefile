# 2-b_compiler/Makefile
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

AS    = $(BUILD)/bin/as
LD    = $(BUILD)/bin/ld

all:	$(BUILD)/bc 

$(BUILD)/lib/crt0.o $(BUILD)/asmlibc.o:
	$(MAKE) -C ../1-asm_libc

# Suppress the default rules
.SUFFIXES:

%.o:	%.s
	$(AS) $<

BC_OBJS  = i386.o scanner.o symtab.o expr.o stmt.o main.o

$(BUILD)/bc:	$(BUILD)/lib/crt0.o $(BUILD)/asmlibc.o $(BC_OBJS)
	$(LD) -o $@ $^

.INTERMEDIATE:	$(LIB_OBJS)
