//  (C) by Remo Dentato (rdentato@gmail.com)
//  SPDX-License-Identifier: MIT

#include "bee.h"

// A simple iterator from 0 to 9
beedef(iter, int n;)
{
   for (bee->n = 0; bee->n < 10; bee->n++) {
     beesleep(1000);
   }
   beereturn;
}

int main(int argc, char *argv[])
{
    int res;
    clock_t wake;
    iter nextitem = beenew(iter);

    // Iterate over the items
    // Note that this is an active loop! It will consume lots of CPU.
    // if you comment out the 'if' you'll see lots of prints when the 
    // beefly() function will return BEE_SLEEP
    while ((res=beefly(nextitem))) {
      if (res != BEE_SLEEP) 
        printf("%d (%d)\n",nextitem->n,res); 
    }
 
    beereset(nextitem);

    // This one uses beewakeat() to sleep enough time to allow the bee to wake up.
    // Sleeping means that your CPU won't overheat :)
    while ((res=beefly(nextitem))) {
      wake=beesleeping(nextitem);
      if ((wake > 0) ) beewakeat(wake);
      printf("%d (%d)\n",nextitem->n,res); 
    }

    beefree(nextitem);
}
