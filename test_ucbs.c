#include <assert.h>
#include <stdio.h>
#include "ucbs.h"

#define BS_ASSERT_SIZE(sz) do {\
  ucbs_t(sz) bs##sz = {.bytes = {0}};\
  assert(sizeof(bs##sz.bytes) == UCBS_ADJ_SZ(sz));\
} while (0)

#define test_access_funcs(l, str, cnt, sz, any, none, all) do { \
  ucbs_t(l) _bs_ = {.bytes = {0}}; \
  ucbs_init_str(_bs_, str); \
  assert(ucbs_count(_bs_) == cnt); \
  assert(ucbs_size(_bs_) == sz); \
  assert(ucbs_any(_bs_) == any); \
  assert(ucbs_none(_bs_) == none); \
  assert(ucbs_all(_bs_) == all); \
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
  ucbs_t(16) bs1 = {.bytes = {0}};
  ucbs_init(bs1, 0U);
  assert(bs1.bytes[0] == 0 && bs1.bytes[1] == 0);
  ucbs_init(bs1, 43690U); // 43690 == b1010101010101010
  for (int i = 0; i < 16; ++i)
    assert(ucbs_test(bs1, i) == ((i % 2) ? true : false));

  ucbs_t(16) bs2 = {.bytes = {0}};
  ucbs_init_str(bs2, "0");
  assert(bs2.bytes[0] == 0 && bs2.bytes[1] == 0);
  ucbs_init_str(bs2, "1010101010101010");
  for (int i = 0; i < 16; ++i)
    assert(ucbs_test(bs2, i) == ((i % 2) ? true : false));
}

static void test_access(void) {
  test_access_funcs(16, "1111111100000000", 8, 16, true, false, false);
  test_access_funcs(16, "1111111111111111", 16, 16, true, false, true);
  test_access_funcs(16, "0", 0, 16, false, true, false);
  test_access_funcs(8, "11111111", 8, 8, true, false, true);
  test_access_funcs(8, "0000", 0, 8, false, true, false);
  test_access_funcs(8, "10000000", 1, 8, true, false, false);
  test_access_funcs(8, "1111111", 7, 8, true, false, false);
}

static void test_operations(void) {
  ucbs_t(16) bs = {.bytes = {0}};
  ucbs_init_str(bs, "0");
  ucbs_setall(bs);
  assert(ucbs_all(bs) == true);
  ucbs_reset(bs, 0);
  assert(ucbs_test(bs, 0) == false);
  ucbs_set(bs, 0);
  assert(ucbs_all(bs) == true);
  for (size_t i = 0; i < 15; i += 2)
    ucbs_flip(bs, i);
  ucbs_t(16) bs_flip = {.bytes = {0}};
  ucbs_init_str(bs_flip, "0101010101010101");
  ucbs_flipall(bs);
  assert(ucbs_equals(bs, bs_flip));
}

int main(void) {
  test_size();
  test_init();
  test_access();
  test_operations();
  return 0;
}
