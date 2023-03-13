//  (C) by Remo Dentato (rdentato@gmail.com)
//  SPDX-License-Identifier: MIT

#include "bee.h"

// A simple iterator from 0 to 9. Any time this coroutine is 
// activated, the bee's variable n will be increased.
beedef(iter, int n; int id;)
{
   // This code is executed only once when the bee is started for the 
   // first time. Here it  could, for example, allocate memory or open files
   printf("Starting bee #%d up\n",bee->id);


   for (bee->n = 0; bee->n < 10; bee->n++) {
     printf("Bee #%d flying with n=%d\n",bee->id,bee->n);
     beeyeld;
     // After a yeld, only the bee's private variables (in this
     // case: n and id) are preserved. Any other variable will
     // be lost.
   }

   beereturn {
     // This is the cleanup code where allocated memory can be freed
     // or opened file can be closed.
     printf("Clean up bee #%d with n=%d\n",bee->id,bee->n);
   };
}

int main(int argc, char *argv[])
{
  int reset = 1;
    iter nxt1 = beenew(iter);
    iter nxt2 = beenew(iter);

    // You access the bee's variable via the pointer dereferencing operator '->' 
    nxt1->id = 1;
    nxt2->id = 2;

    // Just to show that nothing happens when resetting a bee that has never took off!
    beereset(nxt1); 

    // Note that the boolean or '|' is used to avoid the short circuit behaviour of the logical or '||'
    while (beefly(nxt1) | beefly(nxt2)) {

      // We kill the bee #2 after five iterations. You'll see that the cleanup code is executed.
      if (nxt2->n == 5) beekill(nxt2);

      // To show how beereset works, we reset the bee #1 when it reaches 7 for the first time.
      // Note that the cleanup code is executed and then (at the next reactivation) the startup code
      // is executed again.
      if (reset && nxt1->n == 7) {
        beereset(nxt1); // This time it will execute the cleanupcode and will start from 0
        reset = 0;
      } 
    }
    
    beefree(nxt1); // This will invoke the cleanup code
    beefree(nxt2); // This will not invoke the cleanup code since the bee has been killed already!
}
