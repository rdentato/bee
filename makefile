#  (C) by Remo Dentato (rdentato@gmail.com)
#  SPDX-License-Identifier: MIT

CFLAGS=-O2 -Wall

EXAMPLES=iter prodcons fib iter_sleep prodcons_hive

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
