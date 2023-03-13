//  (C) by Remo Dentato (rdentato@gmail.com)
//  SPDX-License-Identifier: MIT

#include "bee.h"

// A simple iterator from 0 to 9
beedef(iter, int n; int id;)
{
   printf("Starting bee %d\n",bee->id);
   for (bee->n = 0; bee->n < 10; bee->n++) {
     printf("Bee %d flying %d\n",bee->id,bee->n);
     beeyeld;
   }

   beereturn {
     printf("Clean up bee %d at %d\n",bee->id,bee->n);
   };
}

int main(int argc, char *argv[])
{
    iter nxt1 = beenew(iter);
    iter nxt2 = beenew(iter);

    nxt1->id = 1;
    nxt2->id = 2;
    // Iterate over the items
    while (beefly(nxt1) | beefly(nxt2)) {
      if (nxt2->n == 5) beekill(nxt2);
    }
    beefree(nxt1);
    beefree(nxt2);
}
