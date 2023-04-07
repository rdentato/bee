
<p width="100%" style="align:center">
<img height="150" src="https://github.com/rdentato/bee/releases/download/candidate/logo_512.png"> <br/>
 </p>

# bee

Simple and clean coroutines in C

(See the [article on DEV.to](https://dev.to/rdentato/coroutines-in-c-1-2-45ig) for more details).

Define `BEE_MAIN` before including `bee.h` in one (and only one of) the source files.

## Reference

  - `beedef(bee_type,...) { ... ; beereturn ;}` Defines a new type of *bee*. After the bee type name, there is a list of variables that will be preserved across yelding. This
     variables can be accessed as fields of the `bee` variable. The `beereturn {...}` instruction is mandatory and can't be used anywhere else
     than at the end of the bee definition. The code at the end is a cleanup code that will be executed when the bee returns (or is killed);

  - `beestop` Stops the execution of the bee. Next call to `beefly()` won't make it fly.

  - `void beeyeld` Suspends the execution of the *bee*. The execution will resume from the next instruction.

  - `void beeyeldwhile(int expr)` Suspends the execution of the bee for as long as the expr is true. If the bee is resumed and the expression is still true, it will yeld, otherwise the execution will resume from the next instruction.

  - `int  beefly(void *bee)` Starts or resume the execution of a *bee*. Returns 1 (`BEE_READY`) if the *bee* can be resumed, 0 (`BEE_DONE`) if the *bee* has completed its task, or 2 (`BEE_SLEEP`) if the bee will sleep for a certain amount of time.

  - `int  beeready(void *bee)` Checks if the *bee* has completed has completed its task (in which case will return 0) or can be resumed (regardless it is sleeping or not).

  - `void beekill(void *bee)` Kills the *bee* so that next time `beefly()` is called, it won't resume.

  - `void beereset(void *bee)` Reset the *bee* so that next time `beefly()` is called, it will start from the beginning.
  
  - `void *beenew(bee_type)` Create a new *bee* of the specified type. Remember that bees are pointers.

  - `void *beefree(void *bee)` Destroy the *bee* and free the associated memory. Returns NULL.

  - `void beesleep(int ms)` Makes the bee unresponsive to fly for at least the next ms milliseconds. Note that due to the way bee sleeping is handled, applications that use bees can't run for more than one year without being reset. 

  - `void beewaitfor(void *bee)` Suspend the process (with `nanoleep()`) to give enough time to the specified bee to awake.

## Beehives
  Beehives are simple scheduler for a group of bees. To use beehives, you have to include "beehive.h" instead of "bee.h"

  - `beehive_t beehivenew()` Creates a new beehive.
   
  - `beehive_t beehivefree(beehive_t hive)` Destrory the *beehive* and free the associated memory. Returns NULL.

  - `int32_t beehiveadd(beehive_t hive, void *bee)` Adds a bee to the beehive. Returns the number of bees.
  
  - `clock_t beehivefly(beehive_t hive)` Send all the bees in the beehive fyling once. If one of the bees complete its task (i.e. returns `BEE_DONE`) it is removed from the hive and freed. Returns `BEEHIVE_DONE` if all the bees have completed their task or the next wake time (0 if any of the bees is not sleeping).

  - `int beehiveswarm(beehive_t hive)` Repeatedly send all the bees flying until all the bees have completed their task. Returns `BEE_DONE`.


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
    beefree(counter);
}
```

Look at `fib.c`, and `prodcons.c` for other examples.
