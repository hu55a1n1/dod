#ifndef uCUTILS_BYTES_H
#define uCUTILS_BYTES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ucbytes_write(_b_, _vptr_) ucbytes_writel_(_b_, _vptr_, sizeof(*_vptr_))
#define ucbytes_read(_b_, _pos_, _vptr_)                                       \
  ucbytes_readl_(_b_, _pos_, _vptr_, sizeof(*_vptr_))
#define ucbytes_print(_b_, _f_)                                                \
  do {                                                                         \
    for (size_t i = 0; i < (_b_)->sz; i++)                                     \
      fprintf(_f_, "%02x", (_b_)->data[i]);                                    \
    fprintf(_f_, "\n");                                                        \
  } while (0)

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

static inline void ucbytes_free(ucbytes_t *b) { free(b); }

static inline int ucbytes_accomodate(ucbytes_t **b, size_t inc) {
  if ((*b)->cap >= ((*b)->sz + inc))
    return 0;
  ucbytes_t *b_ = (ucbytes_t *)realloc(*b, sizeof(*b_) + ((*b)->cap * 2));
  if (!b_)
    return -1;
  *b = b_;
  (*b)->cap *= 2;
  return 0;
}

static inline int ucbytes_writel_(ucbytes_t **b, void *v, size_t l) {
  if (ucbytes_accomodate(b, l) != 0)
    return -1;
  memcpy((*b)->data + (*b)->sz, v, l);
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

static inline void ucbytes_printdl(FILE *file, unsigned char *data, size_t l) {
  for (size_t i = 0; i < l; ++i) {
    fprintf(file, "%02x", data[i]);
  }
  fprintf(file, "\n");
}

#endif // uCUTILS_BYTES_H
