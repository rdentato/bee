//  (C) by Remo Dentato (rdentato@gmail.com)
//  SPDX-License-Identifier: MIT
//  PackageVersion: 1.2.0 RC

#ifndef BEE_H_VERSION
#define BEE_H_VERSION 0x0102000C

#include <stdio.h>
#include <stdlib.h> 
#include <stdint.h>
#include <errno.h>
#include <time.h>

#define BEE_SLEEP 2
#define BEE_READY 1
#define BEE_DONE  0

typedef struct bee_s {
  int (*fly)();
  int32_t line;
  uint32_t wake;
} *bee_t;

#define beeref(bee_type) struct bee_type##_s *


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

#define beestop             goto bee_return

#define beeyeldto(b)      do { \
                            bee->bee_.line = __LINE__ ;  \
                            bee_next = (bee_t)b; \
                            return BEE_READY; \
                            case __LINE__ : ; \
                          } while(0)

#define beeyeld           beeyeldto(NULL)

#define beeyeldwhile(e)   do { \
                            bee->bee_.line = __LINE__ ;  \
                            case __LINE__ : ; \
                            if (e) return BEE_READY; \
                          } while(0)

extern bee_t bee_next;

int  beefly(void *bee);
int  beeready(void *bee);
void beekill(void *bee);
void beereset(void *bee);

#define beenew(bee_type) bee_new(sizeof(struct bee_type##_s), bee_fly_##bee_type)
void *bee_new(size_t size, int (*fly)());
void *beefree(void *bee);


#define beesleep(n)       do { \
                            bee->bee_.line = __LINE__ ;  \
                            bee->bee_.wake = bee_time() + ((n)/10);  \
                            return BEE_READY; \
                            case __LINE__ : ; \
                            if ((bee->bee_.wake > 0) && ( bee_time() < bee->bee_.wake)) return BEE_SLEEP; \
                            bee->bee_.wake = 0; \
                          } while(0)


uint32_t  beesleeping(void *bee);

void bee_wakeat(uint32_t alm);

#define beewaitfor(b) bee_wakeat(beesleeping(b))

extern struct timespec bee_epoch;

uint32_t bee_time();

// --------------------------------------------

#ifdef BEE_MAIN

bee_t bee_next = NULL;

struct timespec bee_epoch = {0};

int beefly(void *bee)
{
  int ret = BEE_DONE;
  bee_next = bee;
  while(bee_next) {
    bee = bee_next; bee_next = NULL;
    ret = ((bee_t)bee)->fly(bee);
    //fprintf(stderr,"FLY: %d ln: %d\n",ret,((bee_t)bee)->line);
  }
  return ret;
}


int beeready(void *bee) {return bee? ((bee_t)bee)->line >= 0 : 0; }

void beekill(void *bee)
{ 
  if (bee) {
    if (((bee_t)bee)->line == 0) { ((bee_t)bee)->line = -1; } // This bee never took off.
    if (((bee_t)bee)->line > 0) { ((bee_t)bee)->line = -2; beefly((bee_t)bee); } // this bee was in flight!
  }
}

void beereset(void *bee) {if (bee) { beekill(bee); ((bee_t)bee)->line =  0; }}

void *bee_new(size_t size, int (*fly)())
{
  bee_t bee = malloc(size); 
  if (bee) { 
    bee->fly = fly;
    bee->line = 0;  
    bee->wake = 0; 
  }

  return bee;
}

void *beefree(void *bee) 
{
  beekill(bee);
  free(bee); 
  return NULL;
}

uint32_t  beesleeping(void *bee) {return bee? ((bee_t)bee)->wake : 0; }

// TIME 

// returns the time in hundrends of seconds from an epoch
uint32_t bee_time()
{
  uint32_t t = 0;
  struct timespec ts;
  if (bee_epoch.tv_sec == 0) {
    clock_gettime(CLOCK_REALTIME,&bee_epoch);
  }
  else {
    clock_gettime(CLOCK_REALTIME,&ts);
    t  = (ts.tv_sec  - bee_epoch.tv_sec)   * 100 
       + (ts.tv_nsec - bee_epoch.tv_nsec) / 10000000;
  }

  return t;
}

static void bee_delay(uint32_t hs) // wait for hundreds of seconds
{
  struct timespec ts;

  ts.tv_sec = hs / 100;
  ts.tv_nsec = (hs % 100) * 10000000;
  nanosleep(&ts, NULL);
  return;
}

void bee_wakeat(uint32_t alm)
{
  uint32_t clk;
  uint32_t wake;
  
  if (alm == 0) return;

  clk = bee_time();
  if (clk < alm) {
    wake = alm-clk;
    bee_delay(wake);
  } 
}

#endif // BEE_MAIN

#endif // BEE_H_VERSION
