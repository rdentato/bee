#  (C) by Remo Dentato (rdentato@gmail.com)
#  SPDX-License-Identifier: MIT

# implicit rules
MAKEFLAGS += --no-builtin-rules

%:
	cd examples; make $*

.PRECIOUS: %.o

# targets

all:
	cd examples; make all

clean:
	cd examples; make clean
