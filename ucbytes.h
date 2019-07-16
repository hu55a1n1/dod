#ifndef uCUTILS_BYTES_H
#define uCUTILS_BYTES_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ucbytes_free(_b_) free(_b_)
#define ucbytes_write(_b_, _vptr_) ucbytes_writel_(_b_, _vptr_, sizeof(*_vptr_))
#define ucbytes_read(_b_, _pos_, _vptr_)                                       \
  ucbytes_readl_(_b_, _pos_, _vptr_, sizeof(*_vptr_))
#define ucbytes_print(_b_, _f_)                                                \
  do {                                                                         \
    for (size_t i = 0; i < (_b_)->sz; i++)                                     \
      fprintf(_f_, "%02x", (_b_)->data[i]);                                    \
    fprintf(_f_, "\n");                                                        \
  } while (0)
#define ucbytes_size(_b_) ((_b_)->sz)
#define ucbytes_set_size(_b_, _nsz_)                                           \
  do {                                                                         \
    (_b_)->sz = _nsz_;                                                         \
  } while (0)
#define ucbytes_capacity(_b_) ((_b_)->cap)
#define ucbytes_data(_b_) ((_b_)->data)
#define ucbytes_accomodate(_bp_, _inc_)                                        \
  ucbytes_reserve(_bp_, (*_bp_)->sz + _inc_)
#define ucbytes_shrink_to_fit(_b_) ucbytes_shrink(_b_, (_b_)->sz)
#define ucbytes_clear(_b_) ((_b_)->sz = 0)

typedef struct {
  size_t sz;
  size_t cap;
  unsigned char data[];
} ucbytes_t;

static inline ucbytes_t *ucbytes_new(size_t sz) {
  ucbytes_t *cb = (ucbytes_t *)malloc(sizeof(*cb) + sz);
  if (!cb) {
    free(cb);
    return NULL;
  }
  cb->sz = 0;
  cb->cap = sz;
  return cb;
}

static inline int ucbytes_reserve(ucbytes_t **b, size_t nsz) {
  if ((*b)->cap >= nsz)
    return 0;
  else if (!(*b)->cap)
    (*b)->cap = nsz;

  size_t ncap = (*b)->cap;
  while (ncap < nsz) {
    ncap *= 2;
  }
  ucbytes_t *b_ = (ucbytes_t *)realloc(*b, sizeof(*b_) + ncap);
  if (!b_)
    return -1;
  *b = b_;
  (*b)->cap = ncap;
  return 0;
}

static inline int ucbytes_writel_(ucbytes_t **b, void *v, size_t l) {
  if (ucbytes_accomodate(b, l) != 0)
    return -1;
  (v != NULL) ? memcpy((*b)->data + (*b)->sz, v, l)
              : memset((*b)->data + (*b)->sz, 0, l);
  (*b)->sz += l;
  return 0;
}

static inline int ucbytes_write_range_atl_(ucbytes_t **b, unsigned char *pos,
                                           unsigned char *start,
                                           unsigned char *end) {
  if (start == NULL)
    return -1;
  size_t idx = 0;
  size_t l = end - start;
  while (pos > (*b)->data) {
    idx++;
    pos -= l;
  }
  if (ucbytes_accomodate(b, l) != 0)
    return -1;
  memmove((*b)->data + ((idx + 1) * l), (*b)->data + (idx * l),
          (*b)->sz - (idx * l));
  memcpy((*b)->data + (idx * l), start, l);
  (*b)->sz += l;
  return 0;
}

static inline int ucbytes_write_atl_(ucbytes_t **b, unsigned char *pos,
                                     unsigned char *val, size_t l) {
  size_t idx = 0;
  while (pos > (*b)->data) {
    idx++;
    pos -= l;
  }
  if (ucbytes_accomodate(b, l) != 0)
    return -1;
  memmove((*b)->data + ((idx + 1) * l), (*b)->data + (idx * l),
          (*b)->sz - (idx * l));
  memcpy((*b)->data + (idx * l), val, l);

  (*b)->sz += l;
  return 0;
}

static inline int ucbytes_readl_(ucbytes_t *b, size_t pos, void *v, size_t l) {
  if (pos >= b->sz)
    return -1;
  memcpy(v, b->data + pos, l);
  b->sz += l;
  return 0;
}

static inline int ucbytes_shrink(ucbytes_t **b, size_t nsz) {
  if (nsz == (*b)->cap)
    return 0;
  else if (nsz > (*b)->cap || nsz < (*b)->sz)
    return -1;
  ucbytes_t *b_ = (ucbytes_t *)realloc(*b, sizeof(*b_) + nsz);
  if (!b_)
    return -2;
  *b = b_;
  (*b)->cap = nsz;
  return 0;
}

#endif // uCUTILS_BYTES_H
