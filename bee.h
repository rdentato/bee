//  (C) by Remo Dentato (rdentato@gmail.com)
//  SPDX-License-Identifier: MIT
//  PackageVersion: 1.2.0 RC

#ifndef BEE_H_VERSION
#define BEE_H_VERSION 0x0102000C

#include <stdio.h>
#include <stdlib.h> 
#include <stdint.h>
#include <time.h>

static inline void beewait(uint32_t ms)
{
  struct timespec ts;
  ts.tv_sec = ms/1000;
  ts.tv_nsec = (ms % 1000) * 1000;
  nanosleep(&ts, NULL);
}

#define BEE_SLEEP 2
#define BEE_READY 1
#define BEE_DONE  0

#define BEE_CLOCKSCALE (CLOCKS_PER_SEC / 1000)

typedef struct bee_s {
  int (*fly)();
  clock_t wake;
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

#define beereturn               } \
                            } \
                            bee_return: bee->bee_.line = -1; \
                            return BEE_DONE

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
                            bee->bee_.line = __LINE__ ;  \
                            bee->bee_.wake = clock();  \
                            bee->bee_.wake += n * BEE_CLOCKSCALE;  \
                            return BEE_READY; \
                            case __LINE__ : ; \
                            if ((bee->bee_.wake > 0) && (clock() < bee->bee_.wake)) return BEE_SLEEP; \
                            bee->bee_.wake = 0; \
                          } while(0)

static inline clock_t   beesleeping(void *bee) {return bee? ((bee_t)bee)->wake : 0; }
static inline int       beefly(void *bee)   {return bee? ((bee_t)bee)->fly(bee) : 0; }
static inline int       beeready(void *bee) {return bee? ((bee_t)bee)->line >= 0 : 0; }
static inline void      beekill(void *bee)  {if (bee) ((bee_t)bee)->line = -1; }
static inline void      beereset(void *bee) {if (bee) ((bee_t)bee)->line =  0; }

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

static inline void *beefree(void *bee) {
  free(bee); return NULL;
}

typedef struct beehive_s {
  void   **bees;
  clock_t  wake;
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
static inline beehive_t beehivefree(beehive_t hive) { free(hive); return NULL;}

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

#define BEEHIVE_DONE ((clock_t)-1)
static inline clock_t beehivefly(beehive_t hive) {

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
  clock_t wake;
  clock_t clk;
  uint32_t w;

  while ((wake = beehivefly(hive)) != BEEHIVE_DONE) {
    clk = clock();
    if (clk > wake) {
      w =  (clk-wake);
      w /= BEE_CLOCKSCALE;
      beewait(w);
    }
  }
  return BEE_DONE;
}

#endif
