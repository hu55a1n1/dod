#ifndef uCUTILS_UCBS_H
#define uCUTILS_UCBS_H

#include "ucret.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define BIT_SET(a, b) ((a) |= (1ULL << (b)))
#define BIT_CLEAR(a, b) ((a) &= ~(1ULL << (b)))
#define BIT_FLIP(a, b) ((a) ^= (1ULL << (b)))
#define BIT_CHECK(a, b) (!!((a) & (1ULL << (b))))

#define ucbs_t(l) struct { \
  uint8_t bytes[(l / 8) + ((l % 8) ? 1 : 0)]; \
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
  for (int _i_ = sizeof(b.bytes); _i_ >= 0; _i_--) \
    for (int _j_ = 7; _j_ >= 0; _j_--) \
      printf("%u", BIT_CHECK(b.bytes[_i_], _j_) ? 1 : 0); \
} while(0)
  
#endif // uCUTILS_UCBS_H
