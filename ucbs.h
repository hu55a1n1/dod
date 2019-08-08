#ifndef uCUTILS_UCBS_H
#define uCUTILS_UCBS_H

#include "ucret.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

typedef size_t ucbs_storage_t;
#define UCBS_T_BIT (sizeof(ucbs_storage_t) * CHAR_BIT)
#define UCBS_ARR_SZ(b) (sizeof(b.bytes)/sizeof(ucbs_storage_t))

#define BIT_SET(a, b) ((a) |= (1ULL << (b)))
#define BIT_CLEAR(a, b) ((a) &= ~(1ULL << (b)))
#define BIT_FLIP(a, b) ((a) ^= (1ULL << (b)))
#define BIT_CHECK(a, b) (!!((a) & (1ULL << (b))))
#define UCBS_ADJ_SZ(l) ((l / UCBS_T_BIT) + ((l % UCBS_T_BIT) ? 1 : 0))

#define ucbs_t(l) struct { \
  ucbs_storage_t bytes[UCBS_ADJ_SZ(l)]; \
  size_t len; \
}
#define ucbs_t_decl(b, l) ucbs_t(l) b = {.bytes = {0}, .len = l };
#define ucbs_t_decl_ptr(b, l) ucbs_t(l) *b = memcpy(malloc(sizeof(*b)), &(ucbs_t(l)){.bytes = {0}, .len = l}, sizeof(*b));

#define ucbs_init(b, u) do { \
  size_t _usz_ = sizeof(b.bytes); \
  ucbs_storage_t* _bp_ = b.bytes + _usz_ - 1; \
  while(_usz_) { \
    for (int _i_ = UCBS_T_BIT - 1; _i_ >= 0; _i_--) {\
      size_t _x_ = ((_usz_ - 1) * UCBS_T_BIT) + _i_;\
      if (BIT_CHECK(u, _x_)) \
        BIT_SET(*_bp_, _x_ % UCBS_T_BIT); \
        } \
    _bp_--; \
    _usz_--; \
    } \
} while(0)

#define ucbs_init_str(b, str) do { \
  size_t _i_ = 0; \
  char *_s_ = &str[strlen(str) - 1]; \
  ucbs_storage_t* _bp_ = b.bytes; \
  while(_s_ && (*_s_ != 0)) { \
    if (*_s_ == '1') BIT_SET(*_bp_, _i_); \
    _i_++; \
    if (_i_ >= UCBS_T_BIT) { \
      _bp_++; \
      _i_ = 0; \
    } \
    _s_--;\
  } \
} while(0)

#define ucbs_print(b) do { \
  size_t _sz_ = UCBS_ARR_SZ(b); \
  for (size_t _i_ = 0; _i_ < _sz_; ++_i_) { \
    for (size_t _j_ = 0; _j_ < UCBS_T_BIT; ++_j_) { \
      printf("%u", BIT_CHECK(b.bytes[_i_], _j_) ? 1 : 0); \
    } \
  } \
} while(0)

// Helpers
#define ucbs_equals(b1, b2) ucbs_equals_(b1.bytes, b1.len, b2.bytes, b2.len, UCBS_ARR_SZ(b1))

// Bit access
#define ucbs_count(b) ucbs_count_(b.bytes, b.len, UCBS_ARR_SZ(b))
#define ucbs_size(b) (UCBS_ARR_SZ(b) * UCBS_T_BIT)
#define ucbs_test(b, pos) BIT_CHECK(b.bytes[pos/UCBS_T_BIT], pos%UCBS_T_BIT)
#define ucbs_any(b) ucbs_any_(b.bytes, b.len, UCBS_ARR_SZ(b))
#define ucbs_none(b) (!ucbs_any_(b.bytes, b.len, UCBS_ARR_SZ(b)))
#define ucbs_all(b) ucbs_all_(b.bytes, b.len, UCBS_ARR_SZ(b))

// Bit operations
#define ucbs_set(b, pos) BIT_SET(b.bytes[pos/UCBS_T_BIT], pos%UCBS_T_BIT)
#define ucbs_setall(b) memset(b.bytes, 0xff, sizeof(b.bytes))
#define ucbs_reset(b, pos) BIT_CLEAR(b.bytes[pos/UCBS_T_BIT], pos%UCBS_T_BIT)
#define ucbs_resetall(b) memset(b.bytes, 0, UCBS_ARR_SZ(b.bytes))
#define ucbs_flip(b, pos) BIT_FLIP(b.bytes[pos/UCBS_T_BIT], pos%UCBS_T_BIT)
#define ucbs_flipall(b) do { \
  size_t _sz_ = UCBS_ARR_SZ(b); \
  for (size_t _i_ = 0; _i_ < _sz_; ++_i_) { \
    for (size_t _j_ = 0; _j_ < UCBS_T_BIT; ++_j_) { \
      if (_i_ >= (_sz_ - 1) && _j_ >= b.len) break; \
      BIT_FLIP(b.bytes[_i_], _j_); \
    } \
  } \
} while (0)

static inline bool
ucbs_equals_(const ucbs_storage_t *bytes1, size_t l1, const ucbs_storage_t *bytes2, size_t l2, size_t sz) {
  if (l1 != l2)
    return false;
  size_t i = 0;
  for (; i < sz - 1; ++i) {
    if (bytes1[i] != bytes2[i]) return false;
  }
  for (size_t j = 0; j < UCBS_T_BIT; ++j) {
    if (j >= l1)
      break;
    else if (BIT_CHECK(bytes1[i], j) != BIT_CHECK(bytes2[i], j))
      return false;
  }
  return true;
}

static inline size_t ucbs_count_(const ucbs_storage_t *bytes, size_t l, size_t sz) {
  size_t count = 0;
  for (size_t i = 0; i < sz; ++i) {
    for (size_t j = 0; j < UCBS_T_BIT; ++j) {
      if (i >= (sz - 1) && j >= l)
        break;
      else if (BIT_CHECK(bytes[i], j)) count++;
    }
  }
  return count;
}

static inline bool ucbs_any_(const ucbs_storage_t *bytes, size_t l, size_t sz) {
  for (size_t i = 0; i < sz; ++i) {
    for (size_t j = 0; j < UCBS_T_BIT; ++j) {
      if (i >= (sz - 1) && j >= l)
        break;
      else if (BIT_CHECK(bytes[i], j) == 1)
        return true;
    }
  }
  return false;
}

static inline bool ucbs_all_(const ucbs_storage_t *bytes, size_t l, size_t sz) {
  for (size_t i = 0; i < sz; ++i) {
    for (size_t j = 0; j < UCBS_T_BIT; ++j) {
      if (i >= (sz - 1) && j >= l)
        break;
      else if (BIT_CHECK(bytes[i], j) != 1)
        return false;
    }
  }
  return true;
}

#endif // uCUTILS_UCBS_H
