//  (C) by Remo Dentato (rdentato@gmail.com)
//  SPDX-License-Identifier: MIT
//  PackageVersion: 1.2.0 RC

#ifndef BEE_H_VERSION
#define BEE_H_VERSION 0x0102000C

#include <stdio.h>
#include <stdlib.h> 
#include <stdint.h>
#include <time.h>
#include <errno.h>

#define BEE_SLEEP 2
#define BEE_READY 1
#define BEE_DONE  0

static inline void beewait(uint32_t ms)
{
  struct timespec ts;

  ts.tv_sec = ms/1000;
  ts.tv_nsec = (ms % 1000) * 1000;
  nanosleep(&ts, NULL);
  return;
}

static inline void beewakeat(uint32_t alm)
{
  uint32_t clk;
  uint32_t wake;
  struct timespec ts;
  
  clock_gettime(CLOCK_REALTIME,&ts); \
  clk = (ts.tv_sec & 0x1FFFFF) * 1000 + (ts.tv_nsec / 1000000);

  if (clk < alm) {
    wake = alm-clk;
    beewait(wake);
  } 
}


typedef struct bee_s {
  int (*fly)();
  uint32_t wake;
  int32_t line;
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
/*
// #define beereturn               } \
//                             } \
//                             bee_return: bee->bee_.line = -1; \
//                             return BEE_DONE
*/

#define beereturn               } \
                            } \
                            bee_return: \
                            if (bee->bee_.line==-1 || bee->bee_.line == 0) return BEE_DONE; \
                            else for (bee->bee_.line=0;;bee->bee_.line=-1) if (bee->bee_.line) return BEE_DONE ; else

#define beestop             goto bee_return

#define beeyeld           do { \
                            bee->bee_.line = __LINE__ ;  \
                            return BEE_READY; \
                            case __LINE__ : ; \
                          } while(0)

#define beeyeldwhile(e)   do { \
                            bee->bee_.line = __LINE__ ;  \
                            case __LINE__ : ; \
                            if (e) return BEE_READY; \
                          } while(0)

#define beesleep(n)       do { \
                            struct timespec ts;\
                            clock_gettime(CLOCK_REALTIME,&ts); \
                            bee->bee_.line = __LINE__ ;  \
                            bee->bee_.wake = (ts.tv_sec & 0x1FFFFF) * 1000 + (ts.tv_nsec / 1000000) + n;  \
                            return BEE_READY; \
                            case __LINE__ : ; \
                            clock_gettime(CLOCK_REALTIME,&ts); \
                            if ((bee->bee_.wake > 0) && ( ((ts.tv_sec & 0x1FFFFF) * 1000 + (ts.tv_nsec / 1000000)) < bee->bee_.wake)) return BEE_SLEEP; \
                            bee->bee_.wake = 0; \
                          } while(0)

static inline uint32_t  beesleeping(void *bee) {return bee? ((bee_t)bee)->wake : 0; }
static inline int       beefly(void *bee)   {return bee? ((bee_t)bee)->fly(bee) : 0; }
static inline int       beeready(void *bee) {return bee? ((bee_t)bee)->line >= 0 : 0; }

static inline void      beekill(void *bee)
{ 
  if (bee) {
    if (((bee_t)bee)->line == 0) { ((bee_t)bee)->line = -1; } // This bee never took off.
    if (((bee_t)bee)->line > 0) { ((bee_t)bee)->line = -2; beefly((bee_t)bee); } // this bee was in flight!
  }
}

static inline void      beereset(void *bee) {if (bee) { beekill(bee); ((bee_t)bee)->line =  0; }}

#define beenew(bee_type) bee_new(sizeof(struct bee_type##_s), bee_fly_##bee_type)
static inline void *bee_new(size_t size, int (*fly)())
{
  bee_t bee = malloc(size); 
  if (bee) {
    bee->line = 0;
    bee->fly  = fly;
    bee->wake = 0;
  }
  return bee;
}

static inline void *beefree(void *bee) 
{
  beekill(bee);
  free(bee); 
  return NULL;
}

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
    if (wake > 0) beewakeat(wake);
  }
  return BEE_DONE;
}

#endif
