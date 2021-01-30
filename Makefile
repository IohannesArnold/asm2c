# Makefile
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

include ./config.make

STAGES = 1-asm_libc 2-b_compiler 3-b_libc 4-c_compiler 5-cpp+cc

all check: $(BUILD_DIR)
	set -e; for n in $(STAGES); do $(MAKE) -r -C $$n $@; done

$(BUILD_DIR): 
	mkdir -p $(BUILD_DIR)/bin $(BUILD_DIR)/lib

clean:
	$(RM) -rf $(BUILD_DIR)
