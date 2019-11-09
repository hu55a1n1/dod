#ifndef DOD_VECT_H
#define DOD_VECT_H

#include "dodret.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constructor and destructor
#define dodvec_t_default(n, sz)                                                \
  { .arr = malloc(n * sz), .l = 0, .szmem = sz, .cap = (n * sz) }
#define dodvec_t_decl(v, t, n) dodvec_t(t) v = dodvec_t_default(n, sizeof(t))
#define dodvec_t_decl_ptr(v, t, n)                                             \
  dodvec_t(t) *v =                                                             \
      memcpy(malloc(sizeof(*v)), &(dodvec_t(t))dodvec_t_default(n, sizeof(t)), \
             sizeof(*v))
#define dodvec_free(v)                                                         \
  do {                                                                         \
    if ((v) != NULL)                                                           \
      free((v)->arr);                                                          \
    free(v);                                                                   \
  } while (0)
#define dodvec_find(v, val)                                                    \
  dodvec_findl((v)->l, val, (v)->szmem, dodvec_data(v))

// Iterators
#define dodvec_begin(v) dodvec_data(v)
#define dodvec_end(v)                                                          \
  (((v)->l == 0) ? dodvec_begin(v) : (dodvec_data(v) + (v)->l))

// capacity
#define dodvec_size(v) (v)->l
#define dodvec_max_size(v) (SIZE_MAX / (v)->szmem)
#define dodvec_capacity(v) ((v)->cap / (v)->szmem)
#define dodvec_empty(v) ((v)->l == 0)
#define dodvec_shrink_to_fit(v)                                                \
  dodvec_shrink_((void **)&(v)->arr, (v)->l, &(v)->cap, (v)->l *(v)->szmem)

// Element access
#define dodvec_data(v) (v)->arr
#define dodvec_at(v, pos) &dodvec_data(v)[pos]
#define dodvec_front(v) dodvec_begin(v)
#define dodvec_back(v)                                                         \
  (dodvec_end(v) == dodvec_begin(v) ? dodvec_front(v) : (dodvec_end(v) - 1))

// Modifiers
#define dodvec_push_back(v, val)                                               \
  dodvec_push_backl_((void **)&(v)->arr, &(v)->l, &(v)->cap, (v)->szmem, val)
#define dodvec_pop_back(v) dodvec_pop_back_(&(v)->l)
#define dodvec_swap(v1, v2)                                                    \
  do {                                                                         \
    void *_arr_ = (v1)->arr;                                                   \
    size_t _l_ = (v1)->l;                                                      \
    size_t _szmem_ = (v1)->szmem;                                              \
    (v1)->arr = (v2)->arr;                                                     \
    (v1)->l = (v2)->l;                                                         \
    (v1)->szmem = (v2)->szmem;                                                 \
    (v2)->arr = _arr_;                                                         \
    (v2)->l = _l_;                                                             \
    (v2)->szmem = _szmem_;                                                     \
  } while (0)
#define dodvec_clear(v) dodvec_clear_(&(v)->l)

#define dodvec_t(t)                                                            \
  struct {                                                                     \
    t *arr;                                                                    \
    size_t l;                                                                  \
    size_t szmem;                                                              \
    size_t cap;                                                                \
  }

#define dodvec_reserve(v, nsz)                                                 \
  dodvec_reserve_((void **)&(v)->arr, &(v)->cap, nsz *(v)->szmem)
static inline dodret_t dodvec_reserve_(void **arr, size_t *cap, size_t ncap) {
  if ((*cap) >= ncap)
    return DODRET_OK;
  else if (!(*cap))
    (*cap) = ncap;

  size_t ncap_ = (*cap);
  while (ncap_ < ncap) {
    ncap_ *= 2;
  }
  void *varr_ = realloc(*arr, sizeof(*arr) + ncap_);
  if (!varr_)
    return DODRET_ENOMEM;
  *arr = varr_;
  (*cap) = ncap_;
  return DODRET_OK;
}

static inline dodret_t dodvec_push_backl_(void **arr, size_t *l, size_t *cap,
                                          size_t szmem, const void *val) {
  if (dodvec_reserve_(arr, cap, ((*l) + 1) * szmem) != 0)
    return DODRET_ENOMEM;
  (val != NULL) ? memcpy((unsigned char *)*arr + ((*l) * szmem), val, szmem)
                : memset((unsigned char *)*arr + ((*l) * szmem), 0, szmem);
  (*l)++;
  return DODRET_OK;
}

static inline void dodvec_pop_back_(size_t *l) {
  if (*l)
    (*l)--;
}

static inline void dodvec_clear_(size_t *l) { *l = 0; }

static inline size_t dodvec_findl(size_t l, const void *val, size_t szmem,
                                  unsigned char *arr) {
  for (size_t i = 0; i < szmem; i++)
    if (!memcmp(arr + (i * szmem), val, szmem))
      return i;
  return l + 1;
}

#define dodvec_erase(v, pos)                                                   \
  dodvec_erase_((unsigned char *)(v)->arr, &(v)->l, (v)->szmem, pos)
static inline dodret_t dodvec_erase_(unsigned char *arr, size_t *l,
                                     size_t szmem, size_t pos) {
  if (!(*l) || pos >= ((*l) * szmem))
    return DODRET_EPARAM;
  size_t li = ((*l) * szmem) - szmem;
  memmove(arr + (pos * szmem), arr + (pos * szmem) + szmem, li);
  (*l)--;
  return DODRET_OK;
}

#define dodvec_resize(v, nsz, val)                                             \
  dodvec_resize_((void **)&(v)->arr, &(v)->l, &(v)->cap, (v)->szmem, nsz, val)
static inline dodret_t dodvec_resize_(void **arr, size_t *l, size_t *cap,
                                      size_t szmem, size_t nsz,
                                      const void *val) {
  if (nsz == *l) {
    return DODRET_OK;
  } else if (nsz > *l) {
    size_t dl = nsz - *l;
    while (dl--)
      if (dodvec_push_backl_(arr, l, cap, szmem, val) < 0)
        return DODRET_ENOMEM;
  } else {
    while (nsz--)
      dodvec_pop_back_(l);
  }
  return DODRET_OK;
}

#define dodvec_assign_range(v, start, end)                                     \
  dodvec_assign_range_((void **)&(v)->arr, &(v)->l, &(v)->cap, (v)->szmem,     \
                       start, end)
static inline dodret_t dodvec_assign_range_(void **arr, size_t *l, size_t *cap,
                                            size_t szmem, const void *start,
                                            const void *end) {
  if (!start || !end || (start > end))
    return DODRET_EPARAM;
  dodvec_clear_(l);
  uintptr_t p = (uintptr_t)start;
  while (p < (uintptr_t)end) {
    if (dodvec_push_backl_(arr, l, cap, szmem, (void *)p) < 0)
      return DODRET_ENOMEM;
    p += szmem;
  }
  return DODRET_OK;
}

#define dodvec_assign_fill(v, n, val)                                          \
  dodvec_assign_fill_((void **)&(v)->arr, &(v)->l, &(v)->cap, (v)->szmem, n,   \
                      val)
static inline dodret_t dodvec_assign_fill_(void **arr, size_t *l, size_t *cap,
                                           size_t szmem, size_t n,
                                           const void *val) {
  dodvec_clear_(l);
  while (n--)
    if (dodvec_push_backl_(arr, l, cap, szmem, val) < 0)
      return DODRET_ENOMEM;
  return DODRET_OK;
}

#define dodvec_insert_range(v, pos, start, end)                                \
  dodvec_insert_range_((void **)&(v)->arr, &(v)->l, &(v)->cap, (v)->szmem,     \
                       pos, start, end)
static inline dodret_t dodvec_insert_range_(void **arr, size_t *l, size_t *cap,
                                            size_t szmem, void *pos,
                                            const void *start,
                                            const void *end) {
  size_t l_ = ((uintptr_t)end - (uintptr_t)start) / szmem;
  size_t idx = 0;
  while (pos > *arr) {
    idx++;
    pos = (unsigned char *)pos - szmem;
  }
  if (dodvec_reserve_(arr, cap, ((*l) + l_) * szmem) != 0)
    return DODRET_ENOMEM;
  memmove((unsigned char *)*arr + ((idx + l_) * szmem),
          (unsigned char *)*arr + (idx * szmem), ((*l) - idx) * szmem);
  memcpy((unsigned char *)*arr + (idx * szmem), start, l_ * szmem);
  *l += l_;
  return DODRET_OK;
}

#define dodvec_insert(v, pos, val)                                             \
  dodvec_insert_((void **)&(v)->arr, &(v)->l, &(v)->cap, (v)->szmem, pos, val)
static inline dodret_t dodvec_insert_(void **arr, size_t *l, size_t *cap,
                                      size_t szmem, void *pos,
                                      const void *val) {
  size_t idx = 0;
  while (pos > *arr) {
    idx++;
    pos = (unsigned char *)pos - szmem;
  }
  if (dodvec_reserve_(arr, cap, ((*l) + 1) * szmem) != 0)
    return DODRET_ENOMEM;
  memmove((unsigned char *)*arr + ((idx + 1) * szmem),
          (unsigned char *)*arr + (idx * szmem), ((*l) - idx) * szmem);
  memcpy((unsigned char *)*arr + (idx * szmem), val, szmem);
  (*l)++;
  return DODRET_OK;
}

static inline dodret_t dodvec_shrink_(void **arr, size_t l, size_t *cap,
                                      size_t nsz) {
  if (nsz == *cap)
    return DODRET_OK;
  else if (nsz > *cap || nsz < l)
    return DODRET_EPARAM;
  void *arr_ = realloc(*arr, sizeof(*arr) + nsz);
  if (!arr_)
    return DODRET_ENOMEM;
  *arr = arr_;
  *cap = nsz;
  return DODRET_OK;
}

#define dodvec_print(v, spec)                                                  \
  do {                                                                         \
    printf("{ \"l\": %lu, \"cap\": %lu, \"arr\": [", (v)->l, (v)->cap);        \
    for (size_t _i_ = 0; _i_ < (v)->l; _i_++)                                  \
      printf(spec ",", (v)->arr[_i_]);                                         \
    printf("]}\n");                                                            \
  } while (0)

#endif // DOD_VECT_H
