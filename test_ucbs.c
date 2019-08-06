#include <assert.h>
#include <stdio.h>
#include "ucbs.h"

#define BS_ASSERT_SIZE(sz) do {\
  ucbs_t(sz) bs##sz = {.bytes = {0}};\
  assert(sizeof(bs##sz.bytes) == UCBS_ADJ_SZ(sz));\
} while (0)

static void test_size(void) {
  BS_ASSERT_SIZE(8);
  BS_ASSERT_SIZE(16);
  BS_ASSERT_SIZE(32);
  BS_ASSERT_SIZE(64);
  BS_ASSERT_SIZE(128);
  BS_ASSERT_SIZE(256);
  BS_ASSERT_SIZE(7);
  BS_ASSERT_SIZE(9);
  BS_ASSERT_SIZE(65536);
}

static void test_init(void) {
  ucbs_t(16) bs = {.bytes = {0}};
  ucbs_init(bs, 0U);
  assert(bs.bytes[0] == 0 && bs.bytes[1] == 0);
  ucbs_init(bs, 43690U); // 43690 == b1010101010101010
  for (int i = 0; i < 16; ++i)
    assert(ucbs_check(bs, i) == ((i % 2) ? 1 : 0));
}

static void test_init_str(void) {
  ucbs_t(16) bs = {.bytes = {0}};
  ucbs_init_str(bs, "0");
  assert(bs.bytes[0] == 0 && bs.bytes[1] == 0);
  ucbs_init_str(bs, "1010101010101010");
  for (int i = 0; i < 16; ++i)
    assert(ucbs_check(bs, i) == ((i % 2) ? 1 : 0));
}

int main(void) {
  test_size();
  test_init();
  test_init_str();
  return 0;
}
