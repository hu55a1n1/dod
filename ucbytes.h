#ifndef uCUTILS_BYTES_H
#define uCUTILS_BYTES_H

#include "ucret.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ucbytes_free(b) free(b)
#define ucbytes_write(b, vptr) ucbytes_writel_(b, vptr, sizeof(*vptr))
#define ucbytes_read(b, pos, vptr)                                       \
  ucbytes_readl_(b, pos, vptr, sizeof(*vptr))
#define ucbytes_print(b, f)                                                \
  do {                                                                         \
    for (size_t i = 0; i < (b)->sz; i++)                                     \
      fprintf(f, "%02x", (b)->data[i]);                                    \
    fprintf(f, "\n");                                                        \
  } while (0)
#define ucbytes_size(b) ((b)->sz)
#define ucbytes_set_size(b, nsz)                                           \
  do {                                                                         \
    (b)->sz = nsz;                                                         \
  } while (0)
#define ucbytes_capacity(b) ((b)->cap)
#define ucbytes_data(b) ((b)->data)
#define ucbytes_accomodate(bp, inc)                                        \
  ucbytes_reserve(bp, (*bp)->sz + inc)
#define ucbytes_shrink_to_fit(b) ucbytes_shrink(b, (b)->sz)
#define ucbytes_clear(b) ((b)->sz = 0)

typedef struct {
    size_t sz;
    size_t cap;
    unsigned char data[];
} ucbytes_t;

static inline ucbytes_t *ucbytes_new(size_t sz) {
  ucbytes_t *cb = (ucbytes_t *) malloc(sizeof(*cb) + sz);
  if (!cb) {
    free(cb);
    return NULL;
  }
  cb->sz = 0;
  cb->cap = sz;
  return cb;
}

static inline ucret_t ucbytes_reserve(ucbytes_t **b, size_t nsz) {
  if ((*b)->cap >= nsz)
    return UCRET_OK;
  else if (!(*b)->cap)
    (*b)->cap = nsz;

  size_t ncap = (*b)->cap;
  while (ncap < nsz) {
    ncap *= 2;
  }
  ucbytes_t *b_ = (ucbytes_t *) realloc(*b, sizeof(*b_) + ncap);
  if (!b_)
    return UCRET_ENOMEM;
  *b = b_;
  (*b)->cap = ncap;
  return UCRET_OK;
}

static inline ucret_t ucbytes_writel_(ucbytes_t **b, const void *v, size_t l) {
  if (ucbytes_accomodate(b, l) != 0)
    return UCRET_ENOMEM;
  (v != NULL) ? memcpy((*b)->data + (*b)->sz, v, l)
              : memset((*b)->data + (*b)->sz, 0, l);
  (*b)->sz += l;
  return UCRET_OK;
}

static inline ucret_t ucbytes_write_range_atl_(ucbytes_t **b,
                                               const unsigned char *pos,
                                               const unsigned char *start,
                                               const unsigned char *end) {
  if (start == NULL)
    return UCRET_EPARAM;
  size_t idx = 0;
  size_t l = end - start;
  while (pos > (*b)->data) {
    idx++;
    pos -= l;
  }
  if (ucbytes_accomodate(b, l) != 0)
    return UCRET_ENOMEM;
  memmove((*b)->data + ((idx + 1) * l), (*b)->data + (idx * l),
          (*b)->sz - (idx * l));
  memcpy((*b)->data + (idx * l), start, l);
  (*b)->sz += l;
  return UCRET_OK;
}

static inline ucret_t ucbytes_write_atl_(ucbytes_t **b,
                                         const unsigned char *pos,
                                         const unsigned char *val, size_t l) {
  size_t idx = 0;
  while (pos > (*b)->data) {
    idx++;
    pos -= l;
  }
  if (ucbytes_accomodate(b, l) != 0)
    return UCRET_ENOMEM;
  memmove((*b)->data + ((idx + 1) * l), (*b)->data + (idx * l),
          (*b)->sz - (idx * l));
  memcpy((*b)->data + (idx * l), val, l);

  (*b)->sz += l;
  return UCRET_OK;
}

static inline ucret_t ucbytes_readl_(ucbytes_t *b, size_t pos, void *v,
                                     size_t l) {
  if (pos >= b->sz)
    return UCRET_EPARAM;
  memcpy(v, b->data + pos, l);
  b->sz += l;
  return UCRET_OK;
}

static inline ucret_t ucbytes_shrink(ucbytes_t **b, size_t nsz) {
  if (nsz == (*b)->cap)
    return UCRET_OK;
  else if (nsz > (*b)->cap || nsz < (*b)->sz)
    return UCRET_EPARAM;
  ucbytes_t *b_ = (ucbytes_t *) realloc(*b, sizeof(*b_) + nsz);
  if (!b_)
    return UCRET_ENOMEM;
  *b = b_;
  (*b)->cap = nsz;
  return UCRET_OK;
}

#endif // uCUTILS_BYTES_H
