#ifndef DOD_DICT_H
#define DOD_DICT_H

#include "dodvec.h"

#define doddict_new(_tk_, _tv_, _n_)                                           \
  doddict_newl(sizeof(_tk_), sizeof(_tv_), _n_)
#define doddict_free(_d_)                                                      \
  do {                                                                         \
    if ((_d_) != NULL) {                                                       \
      dodvec_free((_d_)->keys);                                                \
      dodvec_free((_d_)->vals);                                                \
    }                                                                          \
    free(_d_);                                                                 \
  } while (0)
#define doddict_size(_d_) ((_d_)->keys->l)
#define doddict_push_back(_d_, _k_, _v_)                                       \
  doddict_push_backl(_d_, _k_, sizeof(*_k_), _v_, sizeof(*_v_))
#define doddict_back_read(_d_, _k_, _v_)                                       \
  do {                                                                         \
    memcpy((_k_), dodvec_back((_d_)->keys), sizeof(*(_k_)));                   \
    memcpy((_v_), dodvec_back((_d_)->vals), sizeof(*(_v_)));                   \
  } while (0)
#define doddict_pop_back(_d_)                                                  \
  do {                                                                         \
    dodvec_pop_back((_d_)->keys);                                              \
    dodvec_pop_back((_d_)->vals);                                              \
  } while (0)

typedef struct {
  dodvec_t *keys;
  dodvec_t *vals;
} doddict_t;

static inline doddict_t *doddict_newl(size_t ksz, size_t vsz, size_t nmemb) {
  doddict_t *d = malloc(sizeof(*d));
  if (d) {
    d->keys = dodvec_newl(ksz, ksz * nmemb);
    d->vals = dodvec_newl(vsz, vsz * nmemb);
    if (!d->keys || !d->vals) {
      dodvec_free(d->keys);
      dodvec_free(d->vals);
      free(d);
      return NULL;
    }
  }
  return d;
}

static inline dodret_t doddict_push_backl(doddict_t *d, const void *k,
                                          size_t kl, const void *v, size_t vl) {
  if (dodvec_push_backl(d->keys, k, kl) < 0) {
    return DODRET_ENOMEM;
  } else if (dodvec_push_backl(d->vals, v, vl) < 0) {
    dodvec_pop_back(d->keys);
    return DODRET_ENOMEM;
  }
  return DODRET_OK;
}

#endif // DOD_DICT_H
