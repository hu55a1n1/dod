#ifndef CBUTIL_CBYTE_H
#define CBUTIL_CBYTE_H

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
#define cb_set_sz(_b_, _sz_)                                                   \
  do {                                                                         \
    ((size_t *)_b_)[-2] = _sz_;                                                \
  } while (0)
#define cb_set_p(_b_, _p_)                                                     \
  do {                                                                         \
    (_b_) -= (2 * sizeof(size_t));                                             \
    (_b_) = _p_;                                                               \
    (_b_) += (2 * sizeof(size_t));                                             \
  } while (0)
#define cb_set_cap(_b_, _cap_)                                                 \
  do {                                                                         \
    ((size_t *)_b_)[-1] = _cap_;                                               \
  } while (0)
#define cb_get_sz(_b_) ((_b_) ? ((size_t *)_b_)[-2] : 0)
#define cb_get_cap(_b_) ((_b_) ? ((size_t *)_b_)[-1] : 0)
#define cb_getp(_b_) ((_b_) ? ((size_t *)_b_) - 2 : 0)

typedef unsigned char cbytes;

static inline cbytes *cb_init(size_t sz) {
  cbytes *cb = (unsigned char *)malloc(sz + (sizeof(size_t) * 2));
  if (!cb) {
    free(cb);
    return NULL;
  }
  cb += (sizeof(size_t) * 2);
  cb_set_sz(cb, 0);
  cb_set_cap(cb, sz);
  return cb;
}

static inline void cb_free(cbytes *b) { free(cb_getp(b)); }

static inline int cb_accomodate(cbytes **b, size_t inc) {
  if (cb_get_cap(*b) >= (cb_get_sz(*b) + inc))
    return 0;
  cbytes *b_ = (cbytes *)realloc(cb_getp(*b), cb_get_cap(*b) * 2);
  if (!b_)
    return -1;
  cb_set_p(*b, b_);
  cb_set_cap(*b, cb_get_cap(*b) * 2);
  return 0;
}

static inline int cb_writel_(cbytes **b, void *v, size_t l) {
  if (cb_accomodate(b, l) != 0)
    return -1;
  memcpy(*b + cb_get_sz(*b), v, l);
  cb_set_sz(*b, cb_get_sz(*b) + l);
  return 0;
}

static inline int cb_readl_(cbytes *b, size_t pos, void *v, size_t l) {
  if (pos >= cb_get_sz(b))
    return -1;
  memcpy(v, b + pos, l);
  cb_set_sz(b, cb_get_sz(b) + l);
  return 0;
}

static inline void cb_printdl(FILE *file, unsigned char *data, size_t l) {
  for (size_t i = 0; i < l; ++i) {
    fprintf(file, "%02x", data[i]);
  }
  fprintf(file, "\n");
}

#endif // CBUTIL_CBYTE_H
