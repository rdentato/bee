//  (C) by Remo Dentato (rdentato@gmail.com)
//  SPDX-License-Identifier: MIT

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "bee.h"

#define MAX_FIB 1000000000

#define trace(...) fprintf(stderr,__VA_ARGS__)

/*
  This bee is modeled after an example of another
  coroutine library in C:

  https://github.com/edubart/minicoro

  I wanted to check if I was able to implement the
  same using bee.
*/

// bee->max is the upper limit 
// bee->m is the fibonacci number
// bee->n is to hold the previous number in the sequence

beedef(fib_bee, unsigned long max; unsigned long m; unsigned long n;)
{
  unsigned long tmp;
  bee->m = 1;
  bee->n = 1;
  if (bee->max <=0) bee->max = MAX_FIB;
  while (bee->m <= bee->max) {
    beeyeld;
    tmp = bee->n + bee->m;
    bee->n = bee->m;
    bee->m = tmp;
  } 
  beereturn;
}


int main(int argc, char *argv[])
{
  int counter = 1;
  fib_bee fib = beenew(fib_bee);
  
  if (argc > 1) fib->max = atoi(argv[1]);

  trace("Starting\n");
  while (beefly(fib)) 
     trace("fib %d = %lu\n", counter++, fib->m);
  trace("Completed\n");
  
  beefree(fib);
}
