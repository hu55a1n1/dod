#include "ucbs.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
  ucbs_t bs;
  assert(ucbs_init(&bs, 16) == UCRET_OK);
  ucbs_release(&bs);
  assert(ucbs_init(&bs, 32) == UCRET_OK);
  ucbs_release(&bs);
  assert(ucbs_init(&bs, 64) == UCRET_OK);
  ucbs_release(&bs);
  assert(ucbs_init(&bs, 128) == UCRET_OK);
  ucbs_release(&bs);
  assert(ucbs_init(&bs, 987987349887687687) == UCRET_OK);
  printf("%llu\n", bs.sz);
  ucbs_release(&bs);
  return 0;
}
