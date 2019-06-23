#ifndef CBUTIL_CBYTE_H
#define CBUTIL_CBYTE_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define cb_write(_b_, _vptr_) cb_writel_(_b_, _vptr_, sizeof(*_vptr_))
#define cb_read(_b_, _pos_, _vptr_)                                            \
  cb_readl_(_b_, _pos_, _vptr_, sizeof(*_vptr_))
#define cb_print(_b_, _f_)                                                     \
  do {                                                                         \
    for (size_t i = 0; i < _b_->sz; i++)                                       \
      fprintf(_f_, "%02x", _b_->data[i]);                                      \
    fprintf(_f_, "\n");                                                        \
  } while (0)

typedef struct {
  size_t sz;
  size_t cap;
  uint8_t *data;
} cbytes;

static inline cbytes *cb_init(size_t sz) {
  cbytes *cb = (cbytes *)malloc(sizeof(*cb));
  if (!cb)
    return NULL;
  cb->data = (uint8_t *)malloc(sz);
  if (!cb->data) {
    free(cb);
    return NULL;
  }
  cb->sz = 0;
  cb->cap = sz;
  return cb;
}

static inline void cb_free(cbytes *b) {
  free(b->data);
  free(b);
}

static inline int cb_accomodate(cbytes *b, size_t inc) {
  if (b->cap >= (b->sz + inc))
    return 0;
  uint8_t *b_ = (uint8_t *)realloc(b->data, b->cap * 2);
  if (!b_)
    return -1;
  b->data = b_;
  b->cap *= 2;
  return 0;
}

static inline int cb_writel_(cbytes *b, void *v, size_t l) {
  if (cb_accomodate(b, l) != 0)
    return -1;
  memcpy(b->data + b->sz, v, l);
  b->sz += l;
  return 0;
}

static inline int cb_readl_(cbytes *b, size_t pos, void *v, size_t l) {
  if (pos >= b->sz)
    return -1;
  memcpy(v, b->data + pos, l);
  b->sz += l;
  return 0;
}

static inline void cb_printdl(FILE *file, uint8_t *data, size_t l) {
  for (size_t i = 0; i < l; ++i) {
    fprintf(file, "%02x", data[i]);
  }
  fprintf(file, "\n");
}

#endif // CBUTIL_CBYTE_H
