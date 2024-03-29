//  (C) by Remo Dentato (rdentato@gmail.com)
//  SPDX-License-Identifier: MIT

#define BEE_MAIN
#include "bee.h"

// A simple iterator from 0 to 9
beedef(iter, int n;)
{
   for (bee->n = 0; bee->n < 10; bee->n++) {
     fprintf(stderr,"%d\n",bee->n);
     beesleep(1200);
   }
   beereturn;
}

int main(int argc, char *argv[])
{
    int res;
    iter nxt = beenew(iter);

    // Note that this is an active loop! It will consume lots of CPU.
    while ((res=beefly(nxt))) ;
 
    beereset(nxt); // reuse the same bee

    // This one uses beesync() to sleep enough time to allow the bee to wake up.
    // Sleeping means that your CPU won't overheat :)
    while ((res=beefly(nxt))) {
      if (res == BEE_SLEEP) beesync(nxt);
    }

    beefree(nxt);
}
