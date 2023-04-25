//  (C) by Remo Dentato (rdentato@gmail.com)
//  SPDX-License-Identifier: MIT

#define BEE_MAIN
#include "bee.h"

// A simple iterator from 0 to 9
beedef(iter, int n;)
{
   while(bee->n < 10) {
     beeloopuntil(bee->n & 1) {
       fprintf(stderr,"counting %d\n",bee->n);
       bee->n++;
     }
     printf("BEE: %d\n",bee->n); 
     beeyeld;
   }
   beereturn;
}

int main(int argc, char *argv[])
{
    iter nextitem = beenew(iter);
    // Iterate over the items
    nextitem->n = 0;
    while (beefly(nextitem)) {
      printf("MAIN: %d\n",nextitem->n); 
    }
    beefree(nextitem);
}
