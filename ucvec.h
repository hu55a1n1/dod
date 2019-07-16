#ifndef uCUTILS_VECT_H
#define uCUTILS_VECT_H

#include "ucbytes.h"

#define ucvec_new(_t_, _n_) ucvec_newl(sizeof(_t_), _n_)
#define ucvec_free(_v_)                                                        \
  do {                                                                         \
    if ((_v_) != NULL)                                                         \
      ucbytes_free((_v_)->b);                                                  \
    free(_v_);                                                                 \
  } while (0)
#define ucvec_find(_v_, _val_) ucvec_findl(_v_, _val_, (_v_)->szmem)

// capacity
#define ucvec_size(_v_) ((_v_)->l)
//#define ucvec_max_size(_v_) // todo
#define ucvec_capacity(_v_) (ucbytes_capacity((_v_)->b) / (_v_)->szmem)
#define ucvec_empty(_v_) ((_v_)->l == 0)
#define ucvec_reserve(_v_, _n_) ucbytes_reserve(&(_v_)->b, (_n_) * (_v_)->szmem)
#define ucvec_shrink_to_fit(_v_)                                               \
  ucbytes_shrink(&(_v_)->b, (_v_)->l *(_v_)->szmem)

// Element access
#define ucvec_data(_v_) ucbytes_data((_v_)->b)
#define ucvec_at(_v_, _pos_) (ucvec_data(_v_) + (_pos_ * (_v_)->szmem))
#define ucvec_front(_v_) ucvec_data(_v_)
#define ucvec_back(_v_) (ucvec_data(_v_) + ((_v_)->l - 1) * (_v_)->szmem)
#define ucvec_begin(_v_) ucvec_front(_v_)
#define ucvec_end(_v_) (ucvec_data(_v_) + ((_v_)->l * (_v_)->szmem))

// Modifiers
#define ucvec_push_back(_v_, _val_) ucvec_push_backl(_v_, _val_, (_v_)->szmem)
#define ucvec_pop_back(_v_)                                                    \
  do {                                                                         \
    if ((_v_)->l) {                                                            \
      (_v_)->l--;                                                              \
      (_v_)->b->sz -= (_v_)->szmem;                                            \
    }                                                                          \
  } while (0)
//#define ucvec_insert(_v_) // todo
//#define ucvec_swap(_v1_, _v2_) // todo
#define ucvec_clear(_v_)                                                       \
  do {                                                                         \
    (_v_)->l = 0;                                                              \
    ucbytes_clear((_v_)->b);                                                   \
  } while (0)

typedef struct {
  ucbytes_t *b;
  size_t l;
  size_t szmem;
} ucvec_t;

static inline ucvec_t *ucvec_newl(size_t tsz, size_t n) {
  ucvec_t *v = malloc(sizeof(*v));
  if (v) {
    v->b = ucbytes_new(tsz * n);
    if (!v->b) {
      free(v);
      return NULL;
    }
    v->l = 0;
    v->szmem = tsz;
  }
  return v;
}

static inline int ucvec_push_backl(ucvec_t *v, void *val, size_t l) {
  v->l++;
  return ucbytes_writel_(&v->b, val, l);
}

static inline size_t ucvec_findl(ucvec_t *v, void *val, size_t l) {
  for (size_t i = 0; i < v->l; i++)
    if (!memcmp(ucvec_data(v) + (i * l), val, l))
      return i;
  return v->l + 1;
}

static inline int ucvec_erase(ucvec_t *v, size_t pos) {
  if (!v->l || pos >= ucbytes_size(v->b))
    return -1;
  size_t li = ucbytes_size(v->b) - v->szmem;
  memmove(ucvec_data(v) + (pos * v->szmem),
          ucvec_data(v) + (pos * v->szmem) + v->szmem, li);
  v->l--;
  ucbytes_set_size(v->b, ucbytes_size(v->b) - v->szmem);
  return 0;
}

static inline int ucvec_resize(ucvec_t *v, size_t nsz, void *val) {
  if (nsz == v->l) {
    return 0;
  } else if (nsz > v->l) {
    size_t dl = nsz - v->l;
    while (dl--)
      if (ucvec_push_back(v, val) < 0)
        return -1;
  } else {
    while (nsz--)
      ucvec_pop_back(v);
  }
  return 0;
}

static inline int ucvec_assign_range(ucvec_t *v, void *start, void *end) {
  if (!start || !end)
    return -1;
  else if (start > end)
    return -1;
  ucvec_clear(v);
  uintptr_t p = (uintptr_t)start;
  while (p < (uintptr_t)end) {
    if (ucvec_push_backl(v, (void *)p, v->szmem) < 0)
      return -2;
    p += v->szmem;
  }
  return 0;
}

static inline int ucvec_assign_fill(ucvec_t *v, size_t n, void *val) {
  ucvec_clear(v);
  while (n--)
    if (ucvec_push_backl(v, val, v->szmem) < 0)
      return -1;
  return 0;
}

static inline int ucvec_insert_range(ucvec_t *v, void *pos, void *start,
                                     void *end) {
  size_t l = ((uintptr_t)end - (uintptr_t)start) / v->szmem;
  if (ucbytes_write_range_atl_(&v->b, pos, start, end) < 0)
    return -1;
  v->l += l;
  return 0;
}

static inline int ucvec_insert(ucvec_t *v, void *pos, void *val) {
  if (ucbytes_write_atl_(&v->b, pos, val, v->szmem) < 0)
    return -1;
  v->l++;
  return 0;
}

#endif // uCUTILS_VECT_H
