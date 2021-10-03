//  (C) by Remo Dentato (rdentato@gmail.com)
//  SPDX-License-Identifier: MIT

#include "bee.h"

// A simple iterator from 0 to 9
beedef(iter, int n;)
{
   for (bee->n = 0; bee->n < 10; bee->n++) {
     beeyeld;
   }
   beereturn;
}

int main(int argc, char *argv[])
{
    iter nextitem = beenew(iter);
    // Iterate over the items
    while (beefly(nextitem)) {
      printf("%d\n",nextitem->n); 
    }
    beefree(nextitem);
}
