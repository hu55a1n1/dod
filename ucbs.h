#ifndef uCUTILS_UCBS_H
#define uCUTILS_UCBS_H

#include "ucret.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

#define BIT_SET(a, b) ((a) |= (1ULL << (b)))
#define BIT_CLEAR(a, b) ((a) &= ~(1ULL << (b)))
#define BIT_FLIP(a, b) ((a) ^= (1ULL << (b)))
#define BIT_CHECK(a, b) (!!((a) & (1ULL << (b))))
#define UCBS_ADJ_SZ(l) ((l / 8) + ((l % 8) ? 1 : 0))

#define ucbs_t(l) struct { \
  uint8_t bytes[UCBS_ADJ_SZ(l)]; \
}

#define ucbs_init(b, u) do { \
  size_t _usz_ = sizeof(b.bytes); \
  uint8_t* _bp_ = b.bytes + _usz_ - 1; \
  while(_usz_) { \
    for (int _i_ = 7; _i_ >= 0; _i_--) {\
      size_t _x_ = ((_usz_ - 1) * 8) + _i_;\
      if (BIT_CHECK(u, _x_)) \
        BIT_SET(*_bp_, _x_ % 8); \
        } \
    _bp_--; \
    _usz_--; \
    } \
} while(0)

#define ucbs_init_str(b, str) do { \
  size_t _i_ = 0; \
  char *_s_ = &str[strlen(str) - 1]; \
  uint8_t* _bp_ = b.bytes; \
  while(_s_ && (*_s_ != 0)) { \
    if (*_s_ == '1') BIT_SET(*_bp_, _i_); \
    _i_++; \
    if (_i_ >= 8) { \
      _bp_++; \
      _i_ = 0; \
    } \
    _s_--;\
  } \
} while(0)

#define ucbs_print(b) do { \
  for (int _i_ = sizeof(b.bytes) - 1; _i_ >= 0; _i_--) \
    for (int _j_ = 7; _j_ >= 0; _j_--) \
      printf("%u", BIT_CHECK(b.bytes[_i_], _j_) ? 1 : 0); \
} while(0)

// Helpers
#define ucbs_equals(b1, b2) (sizeof(b1.bytes) == sizeof(b2.bytes) && !memcmp(b1.bytes, b2.bytes, sizeof(b1.bytes)))

// Bit access
#define ucbs_count(b) ucbs_count_(b.bytes, sizeof(b.bytes))
#define ucbs_size(b) (sizeof(b.bytes) * 8)
#define ucbs_test(b, pos) BIT_CHECK(b.bytes[pos/8], pos%8U)
#define ucbs_any(b) ucbs_any_(b.bytes, sizeof(b.bytes))
#define ucbs_none(b) (!ucbs_any_(b.bytes, sizeof(b.bytes)))
#define ucbs_all(b) ucbs_all_(b.bytes, sizeof(b.bytes))

// Bit operations
#define ucbs_set(b, pos) BIT_SET(b.bytes[pos/8], pos%8U)
#define ucbs_setall(b) memset(b.bytes, UINT8_MAX, sizeof(b.bytes))
#define ucbs_reset(b, pos) BIT_CLEAR(b.bytes[pos/8], pos%8U)
#define ucbs_resetall(b) memset(b.bytes, 0, sizeof(b.bytes))
#define ucbs_flip(b, pos) BIT_FLIP(b.bytes[pos/8], pos%8U)
#define ucbs_flipall(b) do { \
  for (size_t _i_ = 0; _i_ < sizeof(b.bytes); ++_i_) \
    for (size_t _j_ = 0; _j_ < 8; ++_j_) \
      BIT_FLIP(b.bytes[_i_], _j_); \
} while (0)

static inline size_t ucbs_count_(const uint8_t *bytes, size_t l) {
  size_t count = 0;
  for (size_t i = 0; i < l; ++i)
    for (size_t j = 0; j < 8; ++j)
      if (BIT_CHECK(bytes[i], j)) count++;
  return count;
}

static inline bool ucbs_any_(const uint8_t *bytes, size_t l) {
  for (size_t i = 0; i < l; ++i)
    if (bytes[i] != 0) return true;
  return false;
}

static inline bool ucbs_all_(const uint8_t *bytes, size_t l) {
  for (size_t i = 0; i < l; ++i)
    for (size_t j = 0; j < 8; ++j)
      if (BIT_CHECK(bytes[i], j) != 1)
        return false;
  return true;
}

#endif // uCUTILS_UCBS_H
