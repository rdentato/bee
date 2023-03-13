#  (C) by Remo Dentato (rdentato@gmail.com)
#  SPDX-License-Identifier: MIT

# implicit rules
MAKEFLAGS += --no-builtin-rules

%:
	cd examples; make $*

all:
	cd examples; make all
