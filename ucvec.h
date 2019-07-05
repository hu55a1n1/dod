#ifndef uCUTILS_VECT_H
#define uCUTILS_VECT_H

#include "ucbytes.h"

#define ucvec_init(_v_, _t_, _rsz_) ucvec_initl(_v_, sizeof(_t_), _rsz_)
#define ucvec_free(_v_) ucbytes_free((_v_)->b)
#define ucvec_push_back(_v_, _val_) ucvec_push_backl(_v_, _val_, (_v_)->szmem)
#define ucvec_back(_v_, _val_)                                                 \
  ucbytes_read((_v_)->b, ((_v_)->l - 1) * (_v_)->szmem, _val_)
#define ucvec_find(_v_, _val_) ucvec_findl(_v_, _val_, (_v_)->szmem)
#define ucvec_empty(_v_) ((_v_)->l == 0)

typedef struct {
  ucbytes_t *b;
  size_t l;
  size_t szmem;
} ucvec_t;

static inline void ucvec_initl(ucvec_t *v, size_t tsz, size_t rsz) {
  v->b = ucbytes_init(rsz);
  v->l = 0;
  v->szmem = tsz;
}

static inline int ucvec_push_backl(ucvec_t *v, void *val, size_t l) {
  v->l++;
  return ucbytes_writel_(&v->b, val, l);
}

static inline void ucvec_pop(ucvec_t *v) {
  if (v->l) {
    v->l--;
    v->b->sz -= v->szmem;
  }
}

static inline size_t ucvec_size(ucvec_t *v) { return v->l; }

static inline size_t ucvec_findl(ucvec_t *v, void *val, size_t l) {
  for (size_t i = 0; i < v->l; i++)
    if (!memcmp(v->b->data + (i * l), val, l))
      return i;
  return v->l + 1;
}

static inline int ucvec_erase(ucvec_t *v, size_t pos) {
  if (!v->l || pos >= v->b->sz)
    return -1;
  size_t li = v->b->sz - v->szmem;
  for (size_t i = pos * v->szmem; i < li; i += v->szmem)
    memcpy(v->b->data + i, v->b->data + i + v->szmem, v->szmem);
  v->l--;
  v->b->sz -= v->szmem;
  return 0;
}

#endif // uCUTILS_VECT_H
