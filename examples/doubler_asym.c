//  (C) by Remo Dentato (rdentato@gmail.com)
//  SPDX-License-Identifier: MIT

#define BEE_MAIN
#include "bee.h"

#define QUE_MAXITEM 16
#include "que.h"

beeref(gen) generator;
beeref(dbl) doubler;

beedef(gen, int n; que_t *q;)
{
  while (bee->n < 10) {
    bee->n++;
    queput(bee->q, bee->n);
    fprintf(stderr,"Added %d...",bee->n);
    beeyeldto(doubler);
  }
  queput(bee->q, -1);
  beeyeldto(doubler);
  beereturn;
}

beedef(dbl, que_t *q;)
{
  int n;

  while ((n=queget(bee->q)) > 0) {
    fprintf(stderr,"%d -> %d\n",n,2*n);
    beeyeldto(generator);
  }
  beereturn;
}

int main(int argc, char *argv[])
{

  generator = beenew(gen);
  doubler = beenew(dbl);
  
  que_t q;

  queinit(&q);

  generator->n = 0;
  generator->q = &q;
  doubler->q = &q;

  beefly(generator);

  beefree(generator);
  beefree(doubler);

}

