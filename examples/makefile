#  (C) by Remo Dentato (rdentato@gmail.com)
#  SPDX-License-Identifier: MIT


_EXE=.exe
SOCKLIB=-lws2_32
STATIC=-static

ifeq "$(COMSPEC)" ""
_EXE=
SOCKLIB=
STATIC=
endif



CFLAGS=-O2 -Wall -I.. $(STATIC)
LIBS=$(SOCKLIB)

EXAMPLES_SRC=$(wildcard *.c)
EXAMPLES=$(EXAMPLES_SRC:.c=)

# implicit rules
MAKEFLAGS += --no-builtin-rules

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%: %.o
	$(CC) $(CFLAGS) $< -o $* $(LIBS)

.PRECIOUS: %.o

# targets

all: $(EXAMPLES)

clean:
	rm -f $(EXAMPLES) $(EXAMPLES:=.exe) $(EXAMPLES:=.o)
