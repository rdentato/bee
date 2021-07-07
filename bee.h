//  (C) by Remo Dentato (rdentato@gmail.com)
//    This software is distributed under the terms of the MIT
//    license: https://opensource.org/licenses/MIT

#ifndef BEE_H
#define BEE_H

#include <stdio.h>
#include <stdlib.h> 
#include <stdint.h>

#define BEE_READY 1
#define BEE_DONE  0

typedef struct bee_s {
  int32_t line;
  int (*fly)();
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
                            bee_return: bee->bee_.line = -1; return BEE_DONE \

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

static inline int  beefly(void *bee)   {return bee? ((bee_t)bee)->fly(bee) : 0; }
static inline int  beeready(void *bee) {return bee? ((bee_t)bee)->line >= 0 : 0; }
static inline void beestop(void *bee)  {if (bee) ((bee_t)bee)->line = -1; }
static inline void beereset(void *bee) {if (bee) ((bee_t)bee)->line =  0; }

#define beenew(bee_type) bee_new(sizeof(struct bee_type##_s), bee_fly_##bee_type)
static inline void *bee_new(size_t size, int (*fly)())
{
  bee_t bee = malloc(size); 
  if (bee) {
    bee->line = 0;
    bee->fly  = fly;
  }
  return bee;
}

static inline void *beefree(void *bee) {free(bee); return NULL;}

#endif
