#ifndef DOD_BYTES_H
#define DOD_BYTES_H

#include "dodret.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define dodbytes_free(b) free(b)
#define dodbytes_write(b, vptr) dodbytes_writel_(b, vptr, sizeof(*vptr))
#define dodbytes_read(b, pos, vptr)                                       \
  dodbytes_readl_(b, pos, vptr, sizeof(*vptr))
#define dodbytes_print(b, f)                                                \
  do {                                                                         \
    for (size_t i = 0; i < (b)->sz; i++)                                     \
      fprintf(f, "%02x", (b)->data[i]);                                    \
    fprintf(f, "\n");                                                        \
  } while (0)
#define dodbytes_size(b) ((b)->sz)
#define dodbytes_set_size(b, nsz)                                           \
  do {                                                                         \
    (b)->sz = nsz;                                                         \
  } while (0)
#define dodbytes_capacity(b) ((b)->cap)
#define dodbytes_data(b) ((b)->data)
#define dodbytes_accomodate(bp, inc)                                        \
  dodbytes_reserve(bp, (*bp)->sz + inc)
#define dodbytes_shrink_to_fit(b) dodbytes_shrink(b, (b)->sz)
#define dodbytes_clear(b) ((b)->sz = 0)

typedef struct {
    size_t sz;
    size_t cap;
    unsigned char data[];
} dodbytes_t;

static inline dodbytes_t *dodbytes_new(size_t sz) {
  dodbytes_t *cb = (dodbytes_t *) malloc(sizeof(*cb) + sz);
  if (!cb) {
    free(cb);
    return NULL;
  }
  cb->sz = 0;
  cb->cap = sz;
  return cb;
}

static inline dodret_t dodbytes_reserve(dodbytes_t **b, size_t nsz) {
  if ((*b)->cap >= nsz)
    return DODRET_OK;
  else if (!(*b)->cap)
    (*b)->cap = nsz;

  size_t ncap = (*b)->cap;
  while (ncap < nsz) {
    ncap *= 2;
  }
  dodbytes_t *b_ = (dodbytes_t *) realloc(*b, sizeof(*b_) + ncap);
  if (!b_)
    return DODRET_ENOMEM;
  *b = b_;
  (*b)->cap = ncap;
  return DODRET_OK;
}

static inline dodret_t dodbytes_writel_(dodbytes_t **b, const void *v, size_t l) {
  if (dodbytes_accomodate(b, l) != 0)
    return DODRET_ENOMEM;
  (v != NULL) ? memcpy((*b)->data + (*b)->sz, v, l)
              : memset((*b)->data + (*b)->sz, 0, l);
  (*b)->sz += l;
  return DODRET_OK;
}

static inline dodret_t dodbytes_write_range_atl_(dodbytes_t **b,
                                                 const unsigned char *pos,
                                                 const unsigned char *start,
                                                 const unsigned char *end) {
  if (start == NULL)
    return DODRET_EPARAM;
  size_t idx = 0;
  size_t l = end - start;
  while (pos > (*b)->data) {
    idx++;
    pos -= l;
  }
  if (dodbytes_accomodate(b, l) != 0)
    return DODRET_ENOMEM;
  memmove((*b)->data + ((idx + 1) * l), (*b)->data + (idx * l),
          (*b)->sz - (idx * l));
  memcpy((*b)->data + (idx * l), start, l);
  (*b)->sz += l;
  return DODRET_OK;
}

static inline dodret_t dodbytes_write_atl_(dodbytes_t **b,
                                           const unsigned char *pos,
                                           const unsigned char *val, size_t l) {
  size_t idx = 0;
  while (pos > (*b)->data) {
    idx++;
    pos -= l;
  }
  if (dodbytes_accomodate(b, l) != 0)
    return DODRET_ENOMEM;
  memmove((*b)->data + ((idx + 1) * l), (*b)->data + (idx * l),
          (*b)->sz - (idx * l));
  memcpy((*b)->data + (idx * l), val, l);

  (*b)->sz += l;
  return DODRET_OK;
}

static inline dodret_t dodbytes_readl_(dodbytes_t *b, size_t pos, void *v,
                                       size_t l) {
  if (pos >= b->sz)
    return DODRET_EPARAM;
  memcpy(v, b->data + pos, l);
  b->sz += l;
  return DODRET_OK;
}

static inline dodret_t dodbytes_shrink(dodbytes_t **b, size_t nsz) {
  if (nsz == (*b)->cap)
    return DODRET_OK;
  else if (nsz > (*b)->cap || nsz < (*b)->sz)
    return DODRET_EPARAM;
  dodbytes_t *b_ = (dodbytes_t *) realloc(*b, sizeof(*b_) + nsz);
  if (!b_)
    return DODRET_ENOMEM;
  *b = b_;
  (*b)->cap = nsz;
  return DODRET_OK;
}

#endif // DOD_BYTES_H
