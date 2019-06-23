#ifndef CBUTIL_CVEC_H
#define CBUTIL_CVEC_H

#include "cbytes.h"

#define vec_push_back(_v_, _val_) vec_push_backl(_v_, _val_, sizeof(*_val_))
#define vec_pop(_v_, _val_) vec_popl(_v_, _val_, sizeof(*_val_))
#define vec_find(_v_, _val_) vec_findl(_v_, _val_, sizeof(*_val_))

typedef struct {
  cbytes *b;
  size_t l;
} vec;

static inline void vec_init(vec *v) { v->b = cb_init(32); }

static inline int vec_push_backl(vec *v, void *val, size_t l) {
  v->l++;
  return cb_writel_(v->b, val, l);
}

static inline int vec_popl(vec *v, void *val, size_t l) {
  v->l--;
  return cb_readl_(v->b, v->l * l, val, l);
}

static inline size_t vec_size(vec *v) { return v->l; }

static inline size_t vec_findl(vec *v, void *val, size_t l) {
  for (int i = 0; i < v->l; i++)
    if (!memcmp(v->b->data + (i * l), val, l))
      return i;
  return v->l + 1;
}

#endif // CBUTIL_CVEC_H
