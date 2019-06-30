#ifndef CBUTIL_CVEC_H
#define CBUTIL_CVEC_H

#include "cbytes.h"

#define vec_init(_v_, _t_, _rsz_) vec_initl(_v_, sizeof(_t_), _rsz_)
#define vec_free(_v_) cb_free((_v_)->b)
#define vec_push_back(_v_, _val_) vec_push_backl(_v_, _val_, (_v_)->szmem)
#define vec_back(_v_, _val_)                                                   \
  cb_read((_v_)->b, ((_v_)->l - 1) * (_v_)->szmem, _val_)
#define vec_find(_v_, _val_) vec_findl(_v_, _val_, (_v_)->szmem)
#define vec_empty(_v_) ((_v_)->l == 0)

typedef struct {
  cbytes *b;
  size_t l;
  size_t szmem;
} vec;

static inline void vec_initl(vec *v, size_t tsz, size_t rsz) {
  v->b = cb_init(rsz);
  v->l = 0;
  v->szmem = tsz;
}

static inline int vec_push_backl(vec *v, void *val, size_t l) {
  v->l++;
  return cb_writel_(&v->b, val, l);
}

static inline void vec_pop(vec *v) {
  if (v->l) {
    v->l--;
    cb_set_sz(v->b, cb_get_sz(v->b) - v->szmem);
  }
}

static inline size_t vec_size(vec *v) { return v->l; }

static inline size_t vec_findl(vec *v, void *val, size_t l) {
  for (size_t i = 0; i < v->l; i++)
    if (!memcmp(v->b + (i * l), val, l))
      return i;
  return v->l + 1;
}

static inline int vec_erase(vec *v, size_t pos) {
  if (!v->l || pos >= cb_get_sz(v->b))
    return -1;
  size_t li = cb_get_sz(v->b) - v->szmem;
  for (size_t i = pos * v->szmem; i < li; i += v->szmem)
    memcpy(v->b + i, v->b + i + v->szmem, v->szmem);
  v->l--;
  cb_set_sz(v->b, cb_get_sz(v->b) - v->szmem);
  return 0;
}

#endif // CBUTIL_CVEC_H
