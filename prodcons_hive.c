//  (C) by Remo Dentato (rdentato@gmail.com)
//  SPDX-License-Identifier: MIT

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "bee.h"

#define MAXITEM 16

// A simple implementation of a queue as a circular buffer
typedef struct {
  int fst;
  int nxt;
  char que[MAXITEM];  
} que_t;

int queisfull(que_t *q)  {return q? (((q->nxt+1) % MAXITEM) == q->fst) : 1;}
int queisempty(que_t *q) {return q? (q->nxt == q->fst) : 1; }
int quelen(que_t *q)     {return (q->nxt - q->fst + MAXITEM) % MAXITEM;}

char queput(que_t *q, char c)
{
  if (queisfull(q)) return '\0';
  q->que[q->nxt] = c;
  q->nxt = (q->nxt + 1) % MAXITEM;
  return c;
}

char queget(que_t *q)
{
  if (queisempty(q)) return '\0';
  char c = q->que[q->fst];
  q->fst = (q->fst + 1) % MAXITEM;
  return c;
}

void queinit(que_t *q) {q->fst = q->nxt = 0;}

#define trace(...) fprintf(stderr,__VA_ARGS__)

/*
  Let's use bees to implement the classical scEnario
  with a producer and a consumer coupled with a queue.

   ╔══════════╗    ━━━━━┓    ╔══════════╗
   ║ producer ║───► ||||┃───►║ consumer ║  
   ╚══════════╝    ━━━━━┛    ╚══════════╝
                   queue

  The example is set so that the producer and the consumer 
  operate at a different "velocity". This may lead to the
  queue filling up and the producer having to wait until
  some space is available or to consumer being able to process
  less elements than it would like to.

  Compile the example and run it as follows:

       echo "ABCDEF" | ./prodcons 2> prodcons.trace

  in the trace output file, you can see how the producer and
  consumer interact. Using different sequence of letters you 
  can drive the behavior of the program.

  For example, using a sequence of all 'D' will make the consumer
  process only one element at the time, meaning that the queue
  will become full and the producer will have to slow down.

  Using a sequence of all 'S', will make the consumer trying to 
  get four character at the time but since the producer is limited to 
  three characters at the time, it won't have enough 
  characters to read.

  Play with different sequences to observe different behaviours.
*/

// The producer reads 3 characters from stdin and puts them
// in the queue, stopping at 'Z', space or EOF.
beedef(producer_bee, que_t *q;)
{
  int c;
  while (1) {
    c = 'A';
    for (int k = 0; c != 'Z' && !queisfull(bee->q) && k < 3 ; k++ ) {
      c = fgetc(stdin);
      if (c == EOF || isspace(c)) c = 'Z';
      trace("PROD puts %c (0x%02X)\n",c,c);
      queput(bee->q, (char)c);
    }
    if (c == 'Z') break;
    trace("PROD yelds\n");
    if (c == 'S') {
      trace("PROD sleeps\n");
      beesleep(2000);
    }
    else
      beeyeld;
  } 
  trace("PROD stops\n");
  beereturn;
}

// The consumer gets from 1 to 4 characters from the
// queue (depending on the first characters in the queue).
// If it's an upper case wovel, prints it as lowercase otherwise
// it prints it as it is.
// Stops at 'Z'

beedef(consumer_bee, que_t *q;)
{
  int c,n;
  while (1) {
    c = queget(bee->q); // c will be '\0' if the queue is empty
    if (c == '\0') {
      trace("%22cCONS sleeps\n",' ');
      beesleep(1500);
    }
    else {
      n = (c & 0x03); // decide how many characters to consume
      while (c) {
        trace("%22cCONS gets %c (0x%02X)\n",' ',c,c);
        if (strchr("AEIOU",c)) c = tolower(c);
        fputc(c, stdout);
        if ((c == 'Z') || (n-- == 0)) break;
        c = queget(bee->q);
      }
      if (c == 'Z') break;
      trace("%22cCONS yelds\n",' ');
      beeyeld;
    }
  }
  fputc('\n',stdout);
  trace("%22cCONS stops\n",' ');
  beereturn;
}

int main(int argc, char *argv[])
{
  que_t q = {0};
  
  producer_bee prod = beenew(producer_bee);
  consumer_bee cons = beenew(consumer_bee);

  beehive_t hive = beehivenew();
  
  // Make prod and cons share the same queue
  prod->q = &q;
  cons->q = &q;

  beehiveadd(hive,prod);
  beehiveadd(hive,cons);

  trace("Starting\n");

  beehiveswarm(hive);

  trace("Completed\n");
  
  beehivefree(hive);

}
