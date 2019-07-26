#ifndef uCUTILS_UCBS_H
#define uCUTILS_UCBS_H

#include "ucret.h"
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>

#define UCBS_SET(_a_, _b_) ((_a_) |= (1ULL << (_b_)))
#define UCBS_CLEAR(_a_, _b_) ((_a_) &= ~(1ULL << (_b_)))
#define UCBS_FLIP(_a_, _b_) ((_a_) ^= (1ULL << (_b_)))
#define UCBS_CHECK(_a_, _b_) (!!((_a_) & (1ULL << (_b_))))
#define UCBS_MASK_SET(_x_, _y_) ((_x_) |= (_y_))
#define UCBS_MASK_CLEAR(_x_, _y_) ((_x_) &= (~(_y_)))
#define UCBS_MASK_FLIP(_x_, _y_) ((_x_) ^= (_y_))
#define UCBS_MASK_CHECK_ALL(_x_, _y_) (((_x_) & (_y_)) == (_y_))
#define UCBS_ASK_CHECK_ANY(_x_, _y_) ((_x_) & (_y_))

#define STACK_BITS (sizeof(uintptr_t) * CHAR_BIT)
#define FITS_ON_STACK(_l_) ((_l_) <= STACK_BITS)
#define IS_ON_STACK(_bs_) ((_bs_)->sz <= STACK_BITS)
#define UDIV_UP(a, b) (((a) + (b)-1) / (b))
#define ALIGN_UP(a, b) (UDIV_UP(a, b) * (b))
#define ALIGN_UP_BITS(a) ALIGN_UP(a, STACK_BITS)
#define MAX(a, b) ((a) > (b) ? (a) : (b))

typedef struct {
  union {
    uintptr_t s;
    unsigned char *h;
  } bits;
  size_t sz;
} ucbs_t;

typedef enum { UCBS_SET = 0, UCBS_CLEAR, UCBS_TOGGLE } bs_op_t;

static inline ucret_t ucbs_init(ucbs_t *bs, size_t bitlen) {
  if (bs == NULL || !bitlen)
    return UCRET_EPARAM;
  if (FITS_ON_STACK(bitlen)) {
    bs->sz = STACK_BITS;
    bs->bits.s = 0;
  } else {
    bs->sz = ALIGN_UP_BITS(bitlen);
    bs->bits.h = calloc(1, (bs->sz / STACK_BITS) + 1);
    if (bs->bits.h == NULL)
      return UCRET_ENOMEM;
  }
  return UCRET_OK;
}

static inline void ucbs_release(ucbs_t *bs) {
  if (bs && !IS_ON_STACK(bs)) {
    free(bs->bits.h);
  }
}



#endif // uCUTILS_UCBS_H
