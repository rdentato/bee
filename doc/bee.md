
# Table of Contents

- [Table of Contents](#table-of-contents)
- [Introduction](#introduction)
- [Coroutine Concepts and Terminology](#coroutine-concepts-and-terminology)
  - [What are Coroutines?](#what-are-coroutines)
    - [Key Concepts and Terminology](#key-concepts-and-terminology)
  - [Implementing Coroutines in C](#implementing-coroutines-in-c)
- [Getting Started with bee](#getting-started-with-bee)
  - [Prerequisites](#prerequisites)
  - [Downloading and Installing the bee Library](#downloading-and-installing-the-bee-library)
  - [A Simple Coroutine Example](#a-simple-coroutine-example)
  - [Next Steps](#next-steps)
- [Coroutine Creation and Management](#coroutine-creation-and-management)
  - [Defining Coroutines](#defining-coroutines)
  - [Creating, Initializing, and Disposing Coroutines](#creating-initializing-and-disposing-coroutines)
- [Coroutine Execution Control](#coroutine-execution-control)
  - [Starting, Suspending and Resuming Coroutines](#starting-suspending-and-resuming-coroutines)
  - [Stopping and Resetting Coroutines](#stopping-and-resetting-coroutines)
  - [Waiting and Sleeping](#waiting-and-sleeping)
    - [On CPU Usage While Waiting.](#on-cpu-usage-while-waiting)
- [Under the hood](#under-the-hood)
- [Reference](#reference)

# Introduction

Welcome to the programmer's manual for the "bee" library, a powerful and easy-to-use C library for implementing coroutines. Coroutines are a versatile programming concept that enables cooperative multitasking and non-preemptive scheduling of tasks in a single-threaded environment. They provide an efficient and intuitive way to manage concurrency in your programs, allowing you to write more responsive and maintainable code.

The "bee" library offers a coroutine implementation based on Duff's Device and a set of preprocessor macros, which together provide a lightweight, fast, and portable solution, without any additional library or language support.

This manual will guide you through the process of understanding, integrating, and using the "bee" library in your C projects.

Throughout this manual, you will find detailed explanations of coroutine concepts, a comprehensive guide to the library's architecture and features, as well as code examples and best practices. 

Whether you are a seasoned C programmer looking to explore the benefits of coroutines or a newcomer eager to learn about this powerful concept, this manual is designed to provide you with the knowledge and tools needed to effectively leverage the "bee" library in your projects. We hope that you find it informative, engaging, and, most importantly, useful in your journey to mastering coroutines in C.

# Coroutine Concepts and Terminology

Coroutines are a powerful programming concept that allows for cooperative multitasking and non-preemptive scheduling of tasks in a single-threaded environment. This chapter will explain the fundamental concepts and terminology associated with coroutines, as well as discuss possible ways to implement them in C.

## What are Coroutines?

Coroutines are a general control structure that allows multiple functions to cooperatively yield and resume execution at specific points, enabling efficient and easy-to-understand management of tasks. Unlike threads, which are preemptively scheduled by the operating system, coroutines are scheduled cooperatively, meaning they voluntarily yield control to other coroutines or the main program.

### Key Concepts and Terminology

- **Coroutine**: A function that can suspend its execution and later resume from the point it left off.
- **Yielding**: The act of voluntarily suspending the execution of a coroutine.
- **Resuming**: Continuing the execution of a coroutine from the point it previously yielded.
- **Coroutine State**: The data and context associated with a coroutine, including local variables and the current execution position.
- **Coroutine Scheduler**: A component responsible for managing the execution and scheduling of coroutines.

## Implementing Coroutines in C

There are several ways to implement coroutines in C, each with its own trade-offs in terms of simplicity, portability, and performance. Some of the most common techniques include:

1. **setjmp and longjmp**: This approach uses the C standard library functions `setjmp` and `longjmp` to save and restore the coroutine's context, including the call stack and registers. It is portable but has some performance overhead due to the need to save and restore the entire context.

2. **Duff's Device**: A technique based on a clever use of C's `switch` statement, which allows jumping into the middle of a loop. This approach is lightweight and fast but can be difficult to maintain due to its unconventional syntax.

3. **Assembly and Stack Manipulation**: In this method, coroutines are implemented using low-level assembly language and direct manipulation of the call stack. This approach is highly efficient and flexible but may be complex to implement and is often platform-specific.

4. **Fibers (Windows) or ucontext (POSIX)**: These platform-specific APIs provide a higher-level interface for creating and managing coroutines, abstracting the low-level details and providing a more straightforward implementation. However, they may be less portable and may have additional overhead compared to other approaches.

The "bee" library is designed to provide a coroutine implementation in C that is efficient, portable, and easy to use. It specifically utilizes the Duff's Device technique for implementing coroutines and leverages a set of preprocessor macros to simplify their usage. This approach allows "bee" to offer a lightweight and fast solution while maintaining a relatively straightforward syntax for developers.

# Getting Started with bee

In this chapter, we will guide you through the process of setting up the "bee" library in your development environment, and provide a simple example to demonstrate the basic usage of coroutines with "bee". By the end of this chapter, you should have a solid foundation to start using the "bee" library in your own projects.

## Prerequisites

Before you begin, ensure that you have the following installed on your system:

1. A C compiler (such as GCC, Clang, or MSVC)
2. A text editor or an Integrated Development Environment (IDE) for writing and editing C code
3. Basic knowledge of C programming, including functions, pointers, and memory management

## Downloading and Installing the bee Library

To start using the "bee" library, follow these steps:

1. Download the latest version of the `bee.h` header  from the official repository (http://gh.dentato.com/bee).
2. Add the header to your project's include path.
3. In one of your source files, define the `BEE_MAIN` macro before including `bee.h`. Typically this is done in the file where the `main()` function is defined.

## A Simple Coroutine Example

Now that you have the "bee" library set up in your development environment, let's create a simple example to illustrate the basic usage of coroutines. In this example, we will create two coroutines that print alternating messages to the console.

Create a new C source file and add the following code:

```C
#include <stdio.h>

#define BEE_MAIN
#include "bee.h"

beedef(print_a, int i;) {
    for (bee->i = 0; bee->i < 5; bee->i += 1) {
        printf("Coroutine A: %d\n", bee->i);
        beeyeld;
    }
}

beedef(print_b, int i;) {
    for (bee->i = 0; bee->i < 5; bee->i += 1) {
        printf("Coroutine B: %d\n", bee->i);
        beeyeld;
    }
}

int main() {
    print_a coro_a = beenew(print_a);
    print_b coro_b = beenew(print_b);

    print_a -> i = 0;
    print_b -> i = 0;

    while (beefly(coro_a) | beefly(coro_b)) ;

    beefree(coro_a);
    beefree(coro_b);

    return 0;
}
```

In this example, we define two coroutines, `print_a` and `print_b`, using the `beedef` macro. Each coroutine prints a message to the console and then yields control back to the main program using the `beeyeld` macro. The main function initializes the coroutines, then runs them in an alternating fashion until both have completed their execution.

To build and run the example, compile the C source file using your C compiler and then execute the resulting binary.

## Next Steps
Now that you have a basic understanding of how to set up and use the "bee" library, you can explore more advanced features and concepts, such as coroutine communication and data sharing, scheduling and prioritization, and integration with other libraries and systems. The following chapters will provide detailed explanations and examples to help you master these topics and make the most of the "bee" library in your projects.

# Coroutine Creation and Management

This chapter focuses on the creation and management of coroutines using the "bee" library. We will explain how to define, create, and manage the lifecycle of coroutines, as well as how to handle their execution control and cleanup.

## Defining Coroutines

In the "bee" library, coroutines are defined using the `beedef` macro, which generates a coroutine type and its associated function. The macro takes two arguments:

1. `bee_type`: The name of the coroutine type.
2. `...`: The local variables that represent the state of the coroutine, separated by semicolons.

Below, tere's an example of how to define a coroutine named `counter` whose state is an integer variable `i` that counts from 0 to 4.
The body of the coroutine should follow the `beedef` macro and must end with the `beereturn` macro, which marks the end of the coroutine code, and the cleanup code if needed. 

Within the body, you can refer to the state of the currently executed coroutine using the `bee` variable.


```C
beedef(counter, int i;)
{
  for (bee->i = 0; bee->i < 5; bee-> += 1) {
      printf("Counter: %d\n", i);
      beeyeld;
  }
  
  beereturn;
}
```

## Creating, Initializing, and Disposing Coroutines
To create a new coroutine instance, you can use the `beenew()` macro, which takes as arguments the type of the bee to create.

```C
counter my_counter = beenew(counter);
```

To initialize the state of the newly created coroutine, you can access its local variable directly:

```C
  my_counter->i = 0;
```

To dispose of a coroutine and free up the resources associated with it, use the `beefree()` function, passing the coroutine instance as an argument:

```C
  my_counter = beefree(my_counter);
```
This function will also execute the cleanup code if it hasn't been executed yet.
The `beefree()` function will always return `NULL` and it's a good practice to set the coroutine to `NULL` when freeing it up.

# Coroutine Execution Control

In this chapter, we will discuss various aspects of controlling the execution flow of coroutines in the "bee" library. We will explore how to suspend and resume coroutines, transfer control between coroutines, and handle coroutine termination.


## Starting, Suspending and Resuming Coroutines

To start or resume the execution of a coroutine, use the `beefly()` function, passing the coroutine instance as an argument:

```C
beefly(my_counter);
```

The `beefly()` function will return `BEE_READY` if the coroutine has yielded or `BEE_DONE` (`0`) if the coroutine has completed its execution.
Trying to resume a coroutine that has already completed its execution will simply return `BEE_DONE` each time.

There are two general strategies for scheduling that are commonly described as:
  
   - *asymmetric coroutines*, that rely on a central scheduler or dispatcher to determine which coroutine to execute next.
   - *symmetric coroutines*, where coroutines directly transfer control to one another;

The `bee` library offers function to implement both:


```c
  beeyeld;                      // yields control back to the scheduler
  beeyeldto(another_coroutine); // yields control to another coroutine
```

Useful wehn using symmetric coroutines, ther is a different form of the `beedef()` function that allow two coroutine know
about the reciprocal exitence.

```C
beedef(consumer) cons; // just to give visibility to the producer that the cons bee will exist.
beedef(producer) prod; // just to give visibility to the consumer that the prod bee will exist.

beedef(producer, int data) 
{
  while (0 <= bee->data && bee->data < 10; ) {
    cons->data = bee->dataM
    bee->data += 1;
    beeyeldto(cons);
  }
  cons->data = -1;
  beeyeldto(cons);
  beereturn;
}

beedef(consumer, int data)
{
  while (cons->data > 0) {
    printf("Data: %d\n",cons->data);
    beeyeldto(prod);
  }
  beereturn;
}

```

## Stopping and Resetting Coroutines
To stop the execution of a coroutine, use the `beestop` macro within the coroutine body:

```C
beestop;
```
The `beekill()` function stops the execution of another coroutine,

```c
  beekill(another_coroutine);
```
To reset a coroutine, use the `beereset()` function, passing the coroutine instance as an argument. 
Next time it will be resumed, its execution will start from the beginning.


```c
beereset(my_counter);
```
These three functions will execute the clean up code for the coroutine, if it has not been already.

## Waiting and Sleeping
Sometimes, a coroutine must wait for an event to happen before being able to continue is job. In those cases, the proper behaviour is to yeld if the event did not happen yet so to allow other coroutines to work properly.

There are several function to help you control the execution of coroutines more precisely, implementing complex behaviors such as conditional yielding, sleeping, and synchronizing coroutines:

    - `beeyeldwhile(e)`: this macro will evaluate the expression `e` and yelds if it is `true`. When the coroutine will be resumed, the expression is evaluated again and, if it is still true, it will yeld again. The execution will move to the next line only when the expression `e` will evaluate to `false`;
    - `beeloopuntil(e)`: this macro will execute its body once, then will evaluate the expression `e`; if it is `true` the execution will move to the next line, otherwise the coroutine will yeld. When the coroutine will be resumed, the body will be executed again and the `e` expression will be re-evaluated, yelding if it is still `false`.
    - `beesleep(n)`: will make the coroutine sleep for n milliseconds. The coroutine will yeld and if it will be resumed, will yeld again until the specified amount of time passed. See below for some restriction on the sleeping time. The `beefly()` function will return `BEE_SLEEP` to signal the fact that the coroutine is sleeping.
    - `beesleeèing(b)`: returns the number of millisecond that a coroutine has yet to sleep.
    - `beesync(b)`: makes the function `sleep()` for enough time to allow the coroutine `b` to wake up.

Note that even if the argument of `beesleep()` is in milliseconds, the actual resolution is in hundrends of second, meaning that the least time a coroutine can sleep is 10 milliseconds. Also, due to implementation details, applications that use `beesleep()` can not be executed continously (i.e. without having them stopped and restarted) for more than 8 months. Should these limitation bee too restrictive, you can always implement your own sync mechanism using `beeyeldwhile()` and `beeloopuntil()`.

### On CPU Usage While Waiting.

The mechanism offered by `bee` are meant to be used in conjunction with other functions to avoid that the application does not spend too much time in idle loops that would needlessly occupy the CPU.

The following example shows how to use the `beesync()` function to make the scheduler process sleep and free up the CPU rather than continuing resuming the coroutine before enough time is passed. Try to execute it while running a CPU usage monitoring utility, you will notice how different the CPU consumption will be in the two cases.

```C
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

    // This is an active loop, it will consume lots of CPU.
    while ((res=beefly(nxt))) ;
 
    beereset(nxt); // reuse the same bee

    // This one uses beesync() to sleep enough time to allow the bee to wake up.
    while ((res=beefly(nxt))) {
      if (res == BEE_SLEEP) beesync(nxt);
    }

    beefree(nxt);
}

```

# Under the hood

To understand the mechanism used by `bee`, you can refer to the following, slightly simplified, code:

```C
typedef struct bee_s {
  int (*fly)();
  int32_t line;
  int32_t aux;
} *bee_t;

#define beedef(bee_type,...) \
                          typedef struct bee_type##_s { \
                            struct bee_s bee_; \
                            __VA_ARGS__ \
                          } *bee_type; \
                          int bee_fly_##bee_type(bee_type bee) \
                          { \
                            if (bee == NULL) goto bee_return; \
                            switch(bee->bee_.line) { \
                                default: goto bee_return; \
                                case  0: 

#define beereturn               } \
                            } \
                            bee_return: \
                            if (bee->bee_.line==-1 || bee->bee_.line == 0) return (bee->bee_.line = -1,BEE_DONE) ; \
                            else for (bee->bee_.line=0;;bee->bee_.line=-1) if (bee->bee_.line) return BEE_DONE; else

#define beeyeld          do { \
                            bee->bee_.line = __LINE__ ;  \
                            return BEE_READY; \
                            case __LINE__ : ; \
                          } while(0)


```
The `bee_s` structure contains the data that is common to all the coroutines:

    - `fly()`: the code that will be executed by the coroutine.
    - `line`: the entry point where the coroutine must be resumed
    - `aux`: used ase a spare for various purposes

Look at the `beeyeld` macro. The `do-while` is there just to ensure safe use of the macro, what's important is that upen yelding we:

    - create the `case` entry point to be used when resuming,
    - set the `line` value of the coroutine to the entry point
    - return to the caller.

Consider what happens after the macro expansion of `beeyeld` (assume it happens to be at line `345` in the code):

```C
                   ---->
  ... code A ...                   ... code A ...
  beeyeld;                         bee->bee_.line = 345;
  ... code B ...                   return BEE_READY;
                         case 345: ... code B ...
```
while executing the code, after `code A` the code will simply return. 
However, the next time the function is called, the `switch` statement will transfer the execution to `case 345:` and will execute `code B`.

In fact the first statement of the `fly` function defined by the macro `beedef`  is:

```C
  switch(bee->bee_.line) { \
      default: goto bee_return; \
      case  0: 
```
The first time the coroutine is executed, the value of `line` will be `0`, meaning that the code right after the `beedef` line will be executed.
The next times, however, the `line` value will make the flow of execution jump to the case defined by the `beeyeld`.

The default case (i.e. when there is no case that correspond to the value of `line`) jumps to the end of the coroutine marked by `beereturn`.

Note how `beereturn` handles the execution of the cleanup code.
If `line` is `-1` it means that the coroutine already exited; if it is `0`, then it never started the execution. In both cases the cleanup code shouldn't be executed and the coroutine just returns.

If the value of `line` is neither `0` or `-1`, then the coroutine was running and the cleanup code must be executed before returning.

# Reference

  - `beedef(bee_type,...) { ... ; beereturn { ... clean up code ...} }` Defines a new type of *bee*. After the bee type name, there is a list of variables that will be preserved across yelding. This
     variables can be accessed as fields of the `bee` variable. The `beereturn {...}` instruction is mandatory and can't be used anywhere else
     than at the end of the bee definition. The code at the end is a cleanup code that will be executed when the bee returns (or is killed);

  - `beedef(bee_type) beename` This form of `beedef()` can be used to declare bee varibles that will be defined later (forward declaration). Useful with `beeyeldto()`.

  - `beestop` Stops the execution of the bee. Next call to `beefly()` won't make it fly.

  - `void beeyeld` Suspends the execution of the *bee*. The execution will resume from the next instruction.

  - `void beeyeldto(bee_t next)` Suspends the execution of the current *bee* and resume the bee *next*.

  - `void beeyeldwhile(int expr)` Suspends the execution of the bee for as long as the expr is true. If the bee is resumed and the expression is still true, it will yeld, otherwise the execution will resume from the next instruction.

  - `int  beefly(void *bee)` Starts or resume the execution of a *bee*. Returns 1 (`BEE_READY`) if the *bee* can be resumed, 0 (`BEE_DONE`) if the *bee* has completed its task, or 2 (`BEE_SLEEP`) if the bee will sleep for a certain amount of time.

  - `int  beeready(void *bee)` Checks if the *bee* has completed has completed its task (in which case will return 0) or can be resumed (regardless it is sleeping or not).

  - `void beekill(void *bee)` Kills the *bee* so that next time `beefly()` is called, it won't resume.

  - `void beereset(void *bee)` Reset the *bee* so that next time `beefly()` is called, it will start from the beginning.
  
  - `void *beenew(bee_type)` Create a new *bee* of the specified type. Remember that bees are pointers.

  - `void *beefree(void *bee)` Destroy the *bee* and free the associated memory. Returns NULL.

  - `void beesleep(int ms)` Makes the bee unresponsive to fly for at least the next ms milliseconds. Note that due to the way bee sleeping is handled, max resolion is in hundreds of seconds (not milliseconds) and applications that use bees can't run for more than one year without being reset. 

  - `void beewaitfor(void *bee)` Suspend the process (with `nanoleep()`) to give enough time to the specified bee to awake.
