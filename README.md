[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/rdentato/bee.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/rdentato/bee/context:cpp)
# bee

Simple and clean coroutines in C

## Reference

  - `beedef(bee_type,...) { ... ; beereturn ;}` Defines a new type of *bee*. After the bee type name, there
     is a list of variables that will be preserved across yelding. This variables can be accessed
     as fields of the `bee` variable. The `beereturn` instruction is mandatory.

  - `void beeyeld()` Suspends the execution of the *bee*. The execution will resume from the next instruction.

  - `void beeyeldwhile(int expr)` Suspends the execution of the bee for as long as the expr is true. The execution will resume from the next instruction.

  - `int  beefly(void *bee)` Starts or resume the execution of a *bee*. Returns `BEE_READY` (1) if the *bee* can be resumed or `BEE_DONE` (0) if the *bee* has completed its task.

  - `int  beeready(void *bee)` Checks if the *bee* will really resume the execution or has completed its task.

  - `void beestop(void *bee)` Stops the *bee* so that next time `beefly()` is called, it won't resume.
  - `void beereset(void *bee)` Reset the *bee* so that next time `beefly()` is called, it will start from the beginning.
  - `void *beenew(bee_type)` Creates a new *bee* of the specified type. Remember that bees are pointer.

  - `void *beefree(void *bee)` Destrory the *bee* and free the associated memory. Returns NULL.

## example

``` C
#include "bee.h"

beedef(iter, int n;)
{
   for (bee->n = 0; bee->n < 10; bee->n++) {
     beeyeld();
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
```
