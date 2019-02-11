# stage-4/Makefile
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

LD = $(BUILD)/ld
AS = $(BUILD)/as

all:	$(BUILD)/bc $(BUILD)/crt0.o $(BUILD)/libc.o $(BUILD)/include

$(LD) $(AS):
	$(MAKE) -C ../stage-3

# Suppress the default rules
.SUFFIXES:

%.o:	%.s $(AS)
	$(AS) $<

%.s:	%.b $(BUILD)/bc0
	$(BUILD)/bc0 -S $<

# LIB_OBJS contain a minimal C library written entirely in assembler.
# LIB0_OBJS adds bootstrap .o files written in assembler that will be replaced.
# LIB1_OBJS adds to and replaces the LIB0_OBJS with versions written in B.

LIB_OBJS = string.o ctype.o unistd.o char.o imath.o
LIB0_OBJS = $(LIB_OBJS) error.o stdio.o memory.o
LIB1_OBJS = $(LIB_OBJS) exit.o output.o input.o malloc.o signal.o string2.o \
  stdarg.o

CC_OBJS  = i386.o scanner.o symtab.o expr.o stmt.o main.o

# libc0.o is the primative libc, written solely in assembler.
# bc0     is the compiler linked against the libc0.o.
# libc.o  is the complete stage-4 libc, with some B code compiled using bc0.
# bc      is the compiler relinked against the new libc.o.

$(BUILD)/libc0.o:	$(LD) $(LIB0_OBJS)
	$(LD) -r -o $(BUILD)/libc0.o $(LIB0_OBJS)

$(BUILD)/crt0.o:	$(AS) crt0.s
	$(AS) crt0.s
	$(CP) crt0.o $@

$(BUILD)/bc0:	$(LD) $(BUILD)/libc0.o $(BUILD)/crt0.o $(CC_OBJS)
	$(LD) -o $(BUILD)/bc0 $(BUILD)/libc0.o crt0.o $(CC_OBJS)

$(BUILD)/libc.o:	$(LD) $(LIB1_OBJS)
	$(LD) -r -o $(BUILD)/libc.o $(LIB1_OBJS)

$(BUILD)/bc:	$(LD) $(BUILD)/libc.o crt0.o $(CC_OBJS)
	$(LD) -o $(BUILD)/bc $(BUILD)/libc.o crt0.o $(CC_OBJS)

.INTERMEDIATE:	$(CC_OBJS) $(LIB1_OBJS) $(LIB0_OBJS) libc0.o bc0

$(BUILD)/include:	./include
	$(CP) -r ./include $(BUILD)/include

check:	check-output check-input check-malloc

check-output: $(BUILD)/bc $(BUILD)/as output.b output.o
	$(CP) output.b output2.b
	$(BUILD)/bc -S output2.b
	$(BUILD)/as output2.s
	$(CMP) output2.o output.o
	$(RM) output2.b output2.o output2.s

check-input: $(BUILD)/bc $(BUILD)/as input.b input.o
	$(CP) input.b input2.b
	$(BUILD)/bc -S input2.b
	$(BUILD)/as input2.s
	$(CMP) input2.o input.o
	$(RM) input2.b input2.o input2.s

check-malloc: $(BUILD)/bc $(BUILD)/as malloc.b malloc.o
	$(CP) malloc.b malloc2.b
	$(BUILD)/bc -S malloc2.b
	$(BUILD)/as malloc2.s
	$(CMP) malloc2.o malloc.o
	$(RM) malloc2.b malloc2.o malloc2.s

clean:
	$(RM) -f $(LIB0_OBJS)
	$(RM) -f $(LIB1_OBJS) output.s input.s malloc.s signal.s libc.o
	$(RM) -f $(CC_OBJS) bc0 bc crt0.o
	$(RM) -f output2.o output2.s input2.o input2.s malloc2.o malloc2.s

