# stage-5/cpp-tests/Makefile
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

include ../../config.make

all:	check

# We have a proper test suite for the preprocessor
CPP_TESTS = empty simple directive hash include macros

check:	$(CPP_TESTS:%=%.run)

# Suppress the default rules
.SUFFIXES:

../$(BUILD)/cpp ../$(BUILD)/cmp:
	$(MAKE) -C .. $(@:../%=%)

%.run:	%.c %.i ../$(BUILD)/cpp ../$(BUILD)/cmp
	../$(BUILD)/cpp  -P $< | ../$(BUILD)/cmp - $(<:%.c=%.i)

