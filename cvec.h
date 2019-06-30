#ifndef CB_CVEC_H
#define CB_CVEC_H

#include "cb.h"

#define cvec_init(_v_, _t_, _rsz_) cvec_initl(_v_, sizeof(_t_), _rsz_)
#define cvec_free(_v_) cb_free((_v_)->b)
#define cvec_push_back(_v_, _val_) cvec_push_backl(_v_, _val_, (_v_)->szmem)
#define cvec_back(_v_, _val_)                                                  \
  cb_read((_v_)->b, ((_v_)->l - 1) * (_v_)->szmem, _val_)
#define cvec_find(_v_, _val_) cvec_findl(_v_, _val_, (_v_)->szmem)
#define cvec_empty(_v_) ((_v_)->l == 0)

typedef struct {
  cb_t *b;
  size_t l;
  size_t szmem;
} cvec_t;

static inline void cvec_initl(cvec_t *v, size_t tsz, size_t rsz) {
  v->b = cb_init(rsz);
  v->l = 0;
  v->szmem = tsz;
}

static inline int cvec_push_backl(cvec_t *v, void *val, size_t l) {
  v->l++;
  return cb_writel_(&v->b, val, l);
}

static inline void cvec_pop(cvec_t *v) {
  if (v->l) {
    v->l--;
    v->b->sz -= v->szmem;
  }
}

static inline size_t cvec_size(cvec_t *v) { return v->l; }

static inline size_t cvec_findl(cvec_t *v, void *val, size_t l) {
  for (size_t i = 0; i < v->l; i++)
    if (!memcmp(v->b->data + (i * l), val, l))
      return i;
  return v->l + 1;
}

static inline int cvec_erase(cvec_t *v, size_t pos) {
  if (!v->l || pos >= v->b->sz)
    return -1;
  size_t li = v->b->sz - v->szmem;
  for (size_t i = pos * v->szmem; i < li; i += v->szmem)
    memcpy(v->b->data + i, v->b->data + i + v->szmem, v->szmem);
  v->l--;
  v->b->sz -= v->szmem;
  return 0;
}

#endif // CB_CVEC_H
