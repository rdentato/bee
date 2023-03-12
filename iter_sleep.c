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
    iter nextitem = beenew(iter);
    // Iterate over the items
    while ((res=beefly(nextitem))) {
      if (res != BEE_SLEEP) printf("%d (%d)\n",nextitem->n,res); 
    }
    beefree(nextitem);
}
