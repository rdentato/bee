//  (C) by Remo Dentato (rdentato@gmail.com)
//  SPDX-License-Identifier: MIT
//  PackageVersion: 1.2.0 RC

#ifndef BEEHIVE_H_VERSION
#define BEEHIVE_H_VERSION 0x0102000C

#include <stdio.h>
#include <stdlib.h> 
#include <stdint.h>
#include <time.h>
#include <errno.h>

#include "bee.h"

typedef struct beehive_s {
  void   **bees;
  uint32_t  wake;
  int32_t  count;
  int32_t  size;
} *beehive_t;

void     *beehivewake(beehive_t hive);
int32_t   beehivestop(beehive_t hive);
void     *beehiveget(beehive_t hive);

static inline beehive_t beehivenew() { 
  beehive_t hive = malloc(sizeof(struct beehive_s));
  if (hive) {
    hive->bees  = NULL;
    hive->count = 0;
    hive->wake  = 0;
    hive->size  = 0;
  }
  return hive;
}
static inline beehive_t beehivefree(beehive_t hive) 
{
  for (int i=0; i< hive->count; i++) {
    beefree(hive->bees[i]);
  }
  free(hive); 
  return NULL;
}

static inline int32_t beehiveadd(beehive_t hive, void *bee)
{
  if (bee) {
    if (hive->count >= hive->size) {
      void  **newhive;
      int32_t newsize = (hive->size+2);
      newsize += newsize / 2;
      newsize += newsize & 1;
      newhive = realloc(hive->bees,newsize*sizeof(void*));
      if (newhive == NULL) return 0; 
      hive->bees = newhive;
      hive->size = newsize;
    }
    hive->bees[hive->count] = bee;
    hive->count++;
  }
  return hive->count;
}

#define BEEHIVE_DONE ((uint32_t)-1)
static inline uint32_t beehivefly(beehive_t hive) {

  if (hive == NULL) return 0;

  int32_t i;
  int res;
  bee_t b;
  
  i = 0;
  hive->wake = UINT32_MAX;

  while (i < hive->count) {
    b = hive->bees[i];
    res = beefly(b);
    if (res == BEE_DONE) {
      beefree(b);
      hive->bees[i] = hive->bees[hive->count-1];
      hive->count--;
    }
    else {
      if (b->wake < hive->wake) hive->wake = b->wake;
      i++;
    }
  }

  return (hive->count == 0) ? BEEHIVE_DONE : hive->wake;
}

static inline int beehiveswarm(beehive_t hive) 
{
  uint32_t wake;

  while ((wake = beehivefly(hive)) != BEEHIVE_DONE) {
    if (wake > 0) bee_wakeat(wake);
  }
  return BEE_DONE;
}

#endif
