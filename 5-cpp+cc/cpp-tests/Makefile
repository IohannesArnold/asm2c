# stage-5/cpp-tests/Makefile
# Copyright (C) 2009-2020 Richard Smith <richard@ex-parrot.com>
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
CPP_TESTS = empty nocpp obj builtin fn simple suppress directive hash \
            rescan macros include includemacro

check:	$(CPP_TESTS:%=%.run)

# Suppress the default rules
.SUFFIXES:

../$(BUILD)/cpp ../$(BUILD)/cmp:
	$(MAKE) -C .. $(@:../%=%)

# The exit status of a pipeline is the status of the rightmost command.
# That means that if the preprocessor exits with non-zero status (e.g.
# because it aborts or segfaults) after it has written everything we 
# expect, the error is ignored and the cmp succeeds.  In bash we'd fix
# this with set -o pipefail, but that doesn't exist in more primitive
# shells.  The 2>&1 ensures that the error message is diverted to the cmp
# where it breaks the comparison forcing a test failure.
%.run:	%.c %.i ../$(BUILD)/cpp ../$(BUILD)/cmp
	../$(BUILD)/cpp -P $< 2>&1 | ../$(BUILD)/cmp - $(<:%.c=%.i)

