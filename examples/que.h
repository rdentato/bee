//  (C) by Remo Dentato (rdentato@gmail.com)
//  SPDX-License-Identifier: MIT

#ifndef QUE_H
#define QUE_H

#ifndef QUE_MAXITEM
#define QUE_MAXITEM 16
#endif

// A simple implementation of a queue as a circular buffer of characters
typedef struct {
  int fst;
  int nxt;
  char que[QUE_MAXITEM];  
} que_t;

#define queisempty(q) (quelen(q) == 0)
#define queisfull(q) (quelen(q) == QUE_MAXITEM)

static int quelen(que_t *q)     {return q? (q->nxt - q->fst + QUE_MAXITEM) % QUE_MAXITEM : 0 ;}

static char queput(que_t *q, char c)
{
  if (c == '\0' || queisfull(q)) return '\0';
  q->que[q->nxt] = c;
  q->nxt = (q->nxt + 1) % QUE_MAXITEM;
  return c;
}

static char queget(que_t *q)
{
  if (queisempty(q)) return '\0';
  char c = q->que[q->fst];
  q->fst = (q->fst + 1) % QUE_MAXITEM;
  return c;
}

static void queinit(que_t *q) {q->fst = q->nxt = 0;}

#endif
