#ifndef DOD_VECT_H
#define DOD_VECT_H

#include "dodbytes.h"

#define dodvec_new(t, n) dodvec_newl(sizeof(t), n)
#define dodvec_free(v)                                                        \
  do {                                                                         \
    if ((v) != NULL)                                                         \
      dodbytes_free((v)->b);                                                  \
    free(v);                                                                 \
  } while (0)
#define dodvec_find(v, val) dodvec_findl(v, val, (v)->szmem)

// capacity
#define dodvec_size(v) ((v)->l)
//#define dodvec_max_size(v) // todo
#define dodvec_capacity(v) (dodbytes_capacity((v)->b) / (v)->szmem)
#define dodvec_empty(v) ((v)->l == 0)
#define dodvec_reserve(v, n) dodbytes_reserve(&(v)->b, (n) * (v)->szmem)
#define dodvec_shrink_to_fit(v)                                               \
  dodbytes_shrink(&(v)->b, (v)->l *(v)->szmem)

// Element access
#define dodvec_data(v) dodbytes_data((v)->b)
#define dodvec_at(v, pos) (dodvec_data(v) + (pos * (v)->szmem))
#define dodvec_front(v) dodvec_data(v)
#define dodvec_back(v) (dodvec_data(v) + ((v)->l - 1) * (v)->szmem)
#define dodvec_begin(v) dodvec_front(v)
#define dodvec_end(v) (dodvec_data(v) + ((v)->l * (v)->szmem))

// Modifiers
#define dodvec_push_back(v, val) dodvec_push_backl(v, val, (v)->szmem)
#define dodvec_pop_back(v)                                                    \
  do {                                                                         \
    if ((v)->l) {                                                            \
      (v)->l--;                                                              \
      (v)->b->sz -= (v)->szmem;                                            \
    }                                                                          \
  } while (0)
#define dodvec_swap(v1, v2)                                                 \
  do {                                                                         \
    dodbytes_t *_b_ = (v1)->b;                                                \
    size_t _l_ = (v1)->l;                                                    \
    size_t _szmem_ = (v1)->szmem;                                            \
    (v1)->b = (v2)->b;                                                     \
    (v1)->l = (v2)->l;                                                     \
    (v1)->szmem = (v2)->szmem;                                             \
    (v2)->b = _b_;                                                           \
    (v2)->l = _l_;                                                           \
    (v2)->szmem = _szmem_;                                                   \
  } while (0)
#define dodvec_clear(v)                                                       \
  do {                                                                         \
    (v)->l = 0;                                                              \
    dodbytes_clear((v)->b);                                                   \
  } while (0)

typedef struct {
    dodbytes_t *b;
    size_t l;
    size_t szmem;
} dodvec_t;

static inline dodvec_t *dodvec_newl(size_t tsz, size_t n) {
  dodvec_t *v = malloc(sizeof(*v));
  if (v) {
    v->b = dodbytes_new(tsz * n);
    if (!v->b) {
      free(v);
      return NULL;
    }
    v->l = 0;
    v->szmem = tsz;
  }
  return v;
}

static inline dodret_t dodvec_push_backl(dodvec_t *v, const void *val, size_t l) {
  v->l++;
  return dodbytes_writel_(&v->b, val, l);
}

static inline size_t dodvec_findl(const dodvec_t *v, const void *val, size_t l) {
  for (size_t i = 0; i < v->l; i++)
    if (!memcmp(dodvec_data(v) + (i * l), val, l))
      return i;
  return v->l + 1;
}

static inline dodret_t dodvec_erase(dodvec_t *v, size_t pos) {
  if (!v->l || pos >= dodbytes_size(v->b))
    return DODRET_EPARAM;
  size_t li = dodbytes_size(v->b) - v->szmem;
  memmove(dodvec_data(v) + (pos * v->szmem),
          dodvec_data(v) + (pos * v->szmem) + v->szmem, li);
  v->l--;
  dodbytes_set_size(v->b, dodbytes_size(v->b) - v->szmem);
  return DODRET_OK;
}

static inline dodret_t dodvec_resize(dodvec_t *v, size_t nsz, const void *val) {
  if (nsz == v->l) {
    return DODRET_OK;
  } else if (nsz > v->l) {
    size_t dl = nsz - v->l;
    while (dl--)
      if (dodvec_push_back(v, val) < 0)
        return DODRET_ENOMEM;
  } else {
    while (nsz--)
      dodvec_pop_back(v);
  }
  return DODRET_OK;
}

static inline dodret_t dodvec_assign_range(dodvec_t *v, const void *start,
                                           const void *end) {
  if (!start || !end || (start > end))
    return DODRET_EPARAM;
  dodvec_clear(v);
  uintptr_t p = (uintptr_t) start;
  while (p < (uintptr_t) end) {
    if (dodvec_push_backl(v, (void *) p, v->szmem) < 0)
      return DODRET_ENOMEM;
    p += v->szmem;
  }
  return DODRET_OK;
}

static inline dodret_t dodvec_assign_fill(dodvec_t *v, size_t n, const void *val) {
  dodvec_clear(v);
  while (n--)
    if (dodvec_push_backl(v, val, v->szmem) < 0)
      return DODRET_ENOMEM;
  return DODRET_OK;
}

static inline dodret_t dodvec_insert_range(dodvec_t *v, const void *pos,
                                           const void *start, const void *end) {
  size_t l = ((uintptr_t) end - (uintptr_t) start) / v->szmem;
  dodret_t r = dodbytes_write_range_atl_(&v->b, pos, start, end);
  if (r < 0)
    return r;
  v->l += l;
  return DODRET_OK;
}

static inline dodret_t dodvec_insert(dodvec_t *v, const void *pos,
                                     const void *val) {
  if (dodbytes_write_atl_(&v->b, pos, val, v->szmem) < 0)
    return DODRET_ENOMEM;
  v->l++;
  return DODRET_OK;
}

#endif // DOD_VECT_H
