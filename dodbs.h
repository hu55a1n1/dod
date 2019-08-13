#ifndef DOD_DODBS_H
#define DOD_DODBS_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

// Bit manipulation
#define BIT_SET(a, b) ((a) |= (1ULL << (b)))
#define BIT_CLEAR(a, b) ((a) &= ~(1ULL << (b)))
#define BIT_FLIP(a, b) ((a) ^= (1ULL << (b)))
#define BIT_CHECK(a, b) (!!((a) & (1ULL << (b))))

// dodbs specific macros and types
typedef size_t dodbs_storage_t;
#define DODBS_T_BIT (sizeof(dodbs_storage_t) * CHAR_BIT)
#define DODBS_ARR_SZ(bs) (sizeof((bs).array)/sizeof(dodbs_storage_t))
#define DODBS_ADJ_SZ(l) ((l / DODBS_T_BIT) + ((l % DODBS_T_BIT) ? 1 : 0))
#define dodbs_t(l) struct { \
  dodbs_storage_t array[DODBS_ADJ_SZ(l)]; \
  size_t len; \
}
#define dodbs_t_default(l) {.array = {0}, .len = l}
#define dodbs_t_decl(bs, l) dodbs_t(l) bs = dodbs_t_default(l)
#define dodbs_t_decl_ptr(bs, l) dodbs_t(l) *bs = memcpy(malloc(sizeof(*bs)), &(dodbs_t(l))dodbs_t_default(l), sizeof(*bs))

// Constructors
#define dodbs_init(bs, u) do { \
  size_t _usz_ = DODBS_ARR_SZ(bs); \
  dodbs_storage_t* _bp_ = &(bs).array[_usz_ - 1]; \
  while(_usz_) { \
    for (int _i_ = DODBS_T_BIT - 1; _i_ >= 0; _i_--) { \
      size_t _x_ = ((_usz_ - 1) * DODBS_T_BIT) + _i_;\
      if (BIT_CHECK(u, _x_)) \
        BIT_SET(*_bp_, _x_ % DODBS_T_BIT); \
        } \
    _bp_--; \
    _usz_--; \
    } \
} while(0)
#define dodbs_init_str(bs, str) do { \
  size_t _i_ = 0; \
  char *_s_ = &str[strlen(str) - 1]; \
  dodbs_storage_t* _bp_ = (bs).array; \
  while(_s_ && (*_s_ != 0)) { \
    if (*_s_ == '1') BIT_SET(*_bp_, _i_); \
    _i_++; \
    if (_i_ >= DODBS_T_BIT) { \
      _bp_++; \
      _i_ = 0; \
    } \
    _s_--;\
  } \
} while(0)

// Helpers
#define dodbs_print(bs) do { \
  size_t _sz_ = DODBS_ARR_SZ(bs); \
  for (size_t _i_ = 0; _i_ < _sz_; ++_i_) { \
    for (size_t _j_ = 0; _j_ < DODBS_T_BIT; ++_j_) { \
      if (_i_ >= (_sz_ - 1) && _j_ >= (bs).len) break; \
      printf("%u", BIT_CHECK((bs).array[_sz_ - _i_ - 1], DODBS_T_BIT - _j_ - 1) ? 1 : 0); \
    } \
  } \
} while(0)
#define dodbs_equals(bs1, bs2) dodbs_equals_((bs1).array, (bs1).len, (bs2).array, (bs2).len, DODBS_ARR_SZ(bs1))

// Bit access
#define dodbs_count(bs) dodbs_count_((bs).array, (bs).len, DODBS_ARR_SZ(bs))
#define dodbs_size(bs) (DODBS_ARR_SZ(bs) * DODBS_T_BIT)
#define dodbs_test(bs, pos) BIT_CHECK((bs).array[pos/DODBS_T_BIT], pos%DODBS_T_BIT)
#define dodbs_any(bs) dodbs_any_((bs).array, (bs).len, DODBS_ARR_SZ(bs))
#define dodbs_none(bs) (!dodbs_any_((bs).array, (bs).len, DODBS_ARR_SZ(bs)))
#define dodbs_all(bs) dodbs_all_((bs).array, (bs).len, DODBS_ARR_SZ(bs))

// Bit operations
#define dodbs_set(bs, pos) BIT_SET((bs).array[pos/DODBS_T_BIT], pos%DODBS_T_BIT)
#define dodbs_setall(bs) ((void) memset((bs).array, 0xff, sizeof((bs).array)))
#define dodbs_reset(bs, pos) BIT_CLEAR((bs).array[pos/DODBS_T_BIT], pos%DODBS_T_BIT)
#define dodbs_resetall(bs) ((void) memset((bs).array, 0, sizeof((bs).array)))
#define dodbs_flip(bs, pos) BIT_FLIP((bs).array[pos/DODBS_T_BIT], pos%DODBS_T_BIT)
#define dodbs_flipall(bs) do { \
  size_t _sz_ = DODBS_ARR_SZ(bs); \
  for (size_t _i_ = 0; _i_ < _sz_; ++_i_) { \
    for (size_t _j_ = 0; _j_ < DODBS_T_BIT; ++_j_) { \
      if (_i_ >= (_sz_ - 1) && _j_ >= (bs).len) break; \
      BIT_FLIP((bs).array[_i_], _j_); \
    } \
  } \
} while (0)

// Bitset operations
#define dodbs_to_string(bs) dodbs_to_string_(bs.array, (bs).len, DODBS_ARR_SZ(bs))
#define dodbs_to_ulong(bs) dodbs_to_ulong_(bs.array)
#define dodbs_to_ullong(bs) dodbs_to_ullong_(bs.array)

// Internal functions
static inline bool
dodbs_equals_(const dodbs_storage_t *bytes1, size_t l1, const dodbs_storage_t *bytes2, size_t l2, size_t sz) {
  if (l1 != l2)
    return false;
  size_t i = 0;
  for (; i < sz - 1; ++i) {
    if (bytes1[i] != bytes2[i]) return false;
  }
  for (size_t j = 0; j < DODBS_T_BIT; ++j) {
    if (j >= l1)
      break;
    else if (BIT_CHECK(bytes1[i], j) != BIT_CHECK(bytes2[i], j))
      return false;
  }
  return true;
}

static inline size_t dodbs_count_(const dodbs_storage_t *array, size_t l, size_t sz) {
  size_t count = 0;
  for (size_t i = 0; i < sz; ++i) {
    for (size_t j = 0; j < DODBS_T_BIT; ++j) {
      if (i >= (sz - 1) && j >= l)
        break;
      else if (BIT_CHECK(array[i], j)) count++;
    }
  }
  return count;
}

static inline bool dodbs_any_(const dodbs_storage_t *array, size_t l, size_t sz) {
  for (size_t i = 0; i < sz; ++i) {
    for (size_t j = 0; j < DODBS_T_BIT; ++j) {
      if (i >= (sz - 1) && j >= l)
        break;
      else if (BIT_CHECK(array[i], j) == 1)
        return true;
    }
  }
  return false;
}

static inline bool dodbs_all_(const dodbs_storage_t *array, size_t l, size_t sz) {
  for (size_t i = 0; i < sz; ++i) {
    for (size_t j = 0; j < DODBS_T_BIT; ++j) {
      if (i >= (sz - 1) && j >= l)
        break;
      else if (BIT_CHECK(array[i], j) != 1)
        return false;
    }
  }
  return true;
}

static inline char *dodbs_to_string_(const dodbs_storage_t *array, size_t l, size_t sz) {
  char *str = malloc(l + 1);
  str[l] = '\0';
  for (size_t i = 0; i < sz; ++i) {
    for (size_t j = 0; j < DODBS_T_BIT; ++j) {
      if (i >= (sz - 1) && j >= l)
        break;
      str[(i * DODBS_T_BIT) + j] = (BIT_CHECK(array[sz - i - 1], DODBS_T_BIT - j - 1) == 1) ? '1' : '0';
    }
  }
  return str;
}

static inline unsigned long dodbs_to_ulong_(const dodbs_storage_t *array) {
  unsigned long tmp;
  memcpy(&tmp, array, sizeof(tmp));
  return tmp;
}

static inline unsigned long long dodbs_to_ullong_(const dodbs_storage_t *array) {
  unsigned long long tmp;
  memcpy(&tmp, array, sizeof(tmp));
  return tmp;
}

#endif // DOD_DODBS_H
