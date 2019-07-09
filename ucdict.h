#ifndef uCUTILS_DICT_H
#define uCUTILS_DICT_H

#include "ucvec.h"

#define ucdict_init(_d_, _tk_, _tv_, _n_)                                      \
  ucdict_initl(_d_, sizeof(_tk_), sizeof(_tv_), _n_)
#define ucdict_free(_d_)                                                       \
  do {                                                                         \
    ucvec_free((_d_)->keys);                                                   \
    ucvec_free((_d_)->vals);                                                   \
  } while (0)
#define ucdict_size(_d_) ((_d_)->keys->l)
#define ucdict_push_back(_d_, _k_, _v_)                                        \
  ucdict_push_backl(_d_, _k_, sizeof(*_k_), _v_, sizeof(*_v_))
#define ucdict_back(_d_, _k_, _v_)                                             \
  do {                                                                         \
    ucvec_back((_d_)->keys, _k_);                                              \
    ucvec_back((_d_)->vals, _v_);                                              \
  } while (0)
#define ucdict_pop(_d_)                                                        \
  do {                                                                         \
    ucvec_pop((_d_)->keys);                                                    \
    ucvec_pop((_d_)->vals);                                                    \
  } while (0)

typedef struct {
  ucvec_t *keys;
  ucvec_t *vals;
} ucdict_t;

static inline void ucdict_initl(ucdict_t *d, size_t ksz, size_t vsz,
                                size_t nmemb) {
  d->keys = ucvec_newl(ksz, ksz * nmemb);
  d->vals = ucvec_newl(vsz, vsz * nmemb);
}

static inline int ucdict_push_backl(ucdict_t *d, void *k, size_t kl, void *v,
                                    size_t vl) {
  ucvec_push_backl(d->keys, k, kl);
  ucvec_push_backl(d->vals, v, vl);
  return 0;
}

#endif // uCUTILS_DICT_H
