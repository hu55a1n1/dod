#ifndef uCUTILS_DICT_H
#define uCUTILS_DICT_H

#include "ucvec.h"

#define ucdict_new(_tk_, _tv_, _n_) ucdict_newl(sizeof(_tk_), sizeof(_tv_), _n_)
#define ucdict_free(_d_)                                                       \
  do {                                                                         \
    if ((_d_) != NULL) {                                                       \
      ucvec_free((_d_)->keys);                                                 \
      ucvec_free((_d_)->vals);                                                 \
    }                                                                          \
    free(_d_);                                                                 \
  } while (0)
#define ucdict_size(_d_) ((_d_)->keys->l)
#define ucdict_push_back(_d_, _k_, _v_)                                        \
  ucdict_push_backl(_d_, _k_, sizeof(*_k_), _v_, sizeof(*_v_))
#define ucdict_back_read(_d_, _k_, _v_)                                        \
  do {                                                                         \
    memcpy((_k_), ucvec_back((_d_)->keys), sizeof(*(_k_)));                    \
    memcpy((_v_), ucvec_back((_d_)->vals), sizeof(*(_v_)));                    \
  } while (0)
#define ucdict_pop_back(_d_)                                                   \
  do {                                                                         \
    ucvec_pop_back((_d_)->keys);                                               \
    ucvec_pop_back((_d_)->vals);                                               \
  } while (0)

typedef struct {
  ucvec_t *keys;
  ucvec_t *vals;
} ucdict_t;

static inline ucdict_t *ucdict_newl(size_t ksz, size_t vsz, size_t nmemb) {
  ucdict_t *d = malloc(sizeof(*d));
  if (d) {
    d->keys = ucvec_newl(ksz, ksz * nmemb);
    d->vals = ucvec_newl(vsz, vsz * nmemb);
    if (!d->keys || !d->vals) {
      ucvec_free(d->keys);
      ucvec_free(d->vals);
      free(d);
      return NULL;
    }
  }
  return d;
}

static inline ucret_t ucdict_push_backl(ucdict_t *d, const void *k, size_t kl,
                                        const void *v, size_t vl) {
  if (ucvec_push_backl(d->keys, k, kl) < 0) {
    return UCRET_ENOMEM;
  } else if (ucvec_push_backl(d->vals, v, vl) < 0) {
    ucvec_pop_back(d->keys);
    return UCRET_ENOMEM;
  }
  return UCRET_OK;
}

#endif // uCUTILS_DICT_H
