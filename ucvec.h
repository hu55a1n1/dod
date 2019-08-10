#ifndef uCUTILS_VECT_H
#define uCUTILS_VECT_H

#include "ucbytes.h"

#define ucvec_new(t, n) ucvec_newl(sizeof(t), n)
#define ucvec_free(v)                                                        \
  do {                                                                         \
    if ((v) != NULL)                                                         \
      ucbytes_free((v)->b);                                                  \
    free(v);                                                                 \
  } while (0)
#define ucvec_find(v, val) ucvec_findl(v, val, (v)->szmem)

// capacity
#define ucvec_size(v) ((v)->l)
//#define ucvec_max_size(v) // todo
#define ucvec_capacity(v) (ucbytes_capacity((v)->b) / (v)->szmem)
#define ucvec_empty(v) ((v)->l == 0)
#define ucvec_reserve(v, n) ucbytes_reserve(&(v)->b, (n) * (v)->szmem)
#define ucvec_shrink_to_fit(v)                                               \
  ucbytes_shrink(&(v)->b, (v)->l *(v)->szmem)

// Element access
#define ucvec_data(v) ucbytes_data((v)->b)
#define ucvec_at(v, pos) (ucvec_data(v) + (pos * (v)->szmem))
#define ucvec_front(v) ucvec_data(v)
#define ucvec_back(v) (ucvec_data(v) + ((v)->l - 1) * (v)->szmem)
#define ucvec_begin(v) ucvec_front(v)
#define ucvec_end(v) (ucvec_data(v) + ((v)->l * (v)->szmem))

// Modifiers
#define ucvec_push_back(v, val) ucvec_push_backl(v, val, (v)->szmem)
#define ucvec_pop_back(v)                                                    \
  do {                                                                         \
    if ((v)->l) {                                                            \
      (v)->l--;                                                              \
      (v)->b->sz -= (v)->szmem;                                            \
    }                                                                          \
  } while (0)
#define ucvec_swap(v1, v2)                                                 \
  do {                                                                         \
    ucbytes_t *_b_ = (v1)->b;                                                \
    size_t _l_ = (v1)->l;                                                    \
    size_t _szmem_ = (v1)->szmem;                                            \
    (v1)->b = (v2)->b;                                                     \
    (v1)->l = (v2)->l;                                                     \
    (v1)->szmem = (v2)->szmem;                                             \
    (v2)->b = _b_;                                                           \
    (v2)->l = _l_;                                                           \
    (v2)->szmem = _szmem_;                                                   \
  } while (0)
#define ucvec_clear(v)                                                       \
  do {                                                                         \
    (v)->l = 0;                                                              \
    ucbytes_clear((v)->b);                                                   \
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

static inline ucret_t ucvec_push_backl(ucvec_t *v, const void *val, size_t l) {
  v->l++;
  return ucbytes_writel_(&v->b, val, l);
}

static inline size_t ucvec_findl(const ucvec_t *v, const void *val, size_t l) {
  for (size_t i = 0; i < v->l; i++)
    if (!memcmp(ucvec_data(v) + (i * l), val, l))
      return i;
  return v->l + 1;
}

static inline ucret_t ucvec_erase(ucvec_t *v, size_t pos) {
  if (!v->l || pos >= ucbytes_size(v->b))
    return UCRET_EPARAM;
  size_t li = ucbytes_size(v->b) - v->szmem;
  memmove(ucvec_data(v) + (pos * v->szmem),
          ucvec_data(v) + (pos * v->szmem) + v->szmem, li);
  v->l--;
  ucbytes_set_size(v->b, ucbytes_size(v->b) - v->szmem);
  return UCRET_OK;
}

static inline ucret_t ucvec_resize(ucvec_t *v, size_t nsz, const void *val) {
  if (nsz == v->l) {
    return UCRET_OK;
  } else if (nsz > v->l) {
    size_t dl = nsz - v->l;
    while (dl--)
      if (ucvec_push_back(v, val) < 0)
        return UCRET_ENOMEM;
  } else {
    while (nsz--)
      ucvec_pop_back(v);
  }
  return UCRET_OK;
}

static inline ucret_t ucvec_assign_range(ucvec_t *v, const void *start,
                                         const void *end) {
  if (!start || !end || (start > end))
    return UCRET_EPARAM;
  ucvec_clear(v);
  uintptr_t p = (uintptr_t) start;
  while (p < (uintptr_t) end) {
    if (ucvec_push_backl(v, (void *) p, v->szmem) < 0)
      return UCRET_ENOMEM;
    p += v->szmem;
  }
  return UCRET_OK;
}

static inline ucret_t ucvec_assign_fill(ucvec_t *v, size_t n, const void *val) {
  ucvec_clear(v);
  while (n--)
    if (ucvec_push_backl(v, val, v->szmem) < 0)
      return UCRET_ENOMEM;
  return UCRET_OK;
}

static inline ucret_t ucvec_insert_range(ucvec_t *v, const void *pos,
                                         const void *start, const void *end) {
  size_t l = ((uintptr_t) end - (uintptr_t) start) / v->szmem;
  ucret_t r = ucbytes_write_range_atl_(&v->b, pos, start, end);
  if (r < 0)
    return r;
  v->l += l;
  return UCRET_OK;
}

static inline ucret_t ucvec_insert(ucvec_t *v, const void *pos,
                                   const void *val) {
  if (ucbytes_write_atl_(&v->b, pos, val, v->szmem) < 0)
    return UCRET_ENOMEM;
  v->l++;
  return UCRET_OK;
}

#endif // uCUTILS_VECT_H
