//  (C) by Remo Dentato (rdentato@gmail.com)
//    This software is distributed under the terms of the MIT
//    license: https://opensource.org/licenses/MIT

#include "bee.h"

beedef(iter, int n;)
{
   for (bee->n = 0; bee->n < 10; bee->n++) {
     beeyeld;
   }
   beereturn;
}

int main(int argc, char *argv[])
{
    iter counter = beenew(iter);
    while (beefly(counter)) {
      printf("%d\n",counter->n); 
    }
}
