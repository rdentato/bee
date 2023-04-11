#  (C) by Remo Dentato (rdentato@gmail.com)
#  SPDX-License-Identifier: MIT

CFLAGS=-O2 -Wall -I.. -static

EXAMPLES_SRC=$(wildcard *.c)
EXAMPLES=$(EXAMPLES_SRC:.c=)

# implicit rules
MAKEFLAGS += --no-builtin-rules

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%: %.o
	$(CC) $(CFLAGS) $< -o $* 

.PRECIOUS: %.o

# targets

all: $(EXAMPLES)

clean:
	rm -f $(EXAMPLES) $(EXAMPLES:=.exe) $(EXAMPLES:=.o)
