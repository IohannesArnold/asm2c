# Makefile

# Copyright (C) 2009, 2011 Richard Smith <richard@ex-parrot.com>
# All rights reserved.

STAGES = 2

all init check:
	set -e; for n in `seq 0 $(STAGES)`; do $(MAKE) -C stage-$$n $@; done

clean:
	for n in `seq 0 $(STAGES)`; do $(MAKE) -C stage-$$n $@; done
