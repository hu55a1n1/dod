#include "ucbs.h"
#include "uctest.h"
#include <assert.h>
#include <stdio.h>

#define ULL_BIN_STR "11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111"

#define BS_ASSERT_SIZE(sz) do {\
  ucbs_t_decl(bs##sz, sz);\
  assert(sizeof(bs##sz.array) == (UCBS_ADJ_SZ(sz) * sizeof(ucbs_storage_t))); \
} while (0)

#define test_access_funcs(l, str, cnt, any, none, all) do { \
  ucbs_t_decl(_bs_, l); \
  ucbs_init_str(_bs_, str); \
  assert(ucbs_count(_bs_) == cnt); \
  assert(ucbs_size(_bs_) == UCBS_ARR_SZ(_bs_) * UCBS_T_BIT); \
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
  ucbs_t_decl(bs1, 16);
  ucbs_init(bs1, 43690U); // 43690 == b1010101010101010
  for (int i = 0; i < 16; ++i)
    assert(ucbs_test(bs1, i) == ((i % 2) ? true : false));

  ucbs_t_decl(bs2, 16);
  ucbs_init_str(bs2, "1010101010101010");
  for (int i = 0; i < 16; ++i)
    assert(ucbs_test(bs2, i) == ((i % 2) ? true : false));
}

static void test_bit_access(void) {
  test_access_funcs(16, "1111111100000000", 8, true, false, false);
  test_access_funcs(16, "1111111111111111", 16, true, false, true);
  test_access_funcs(16, "0", 0, false, true, false);
  test_access_funcs(8, "11111111", 8, true, false, true);
  test_access_funcs(8, "0000", 0, false, true, false);
  test_access_funcs(8, "10000000", 1, true, false, false);
  test_access_funcs(8, "1111111", 7, true, false, false);
}

static void test_bit_ops(void) {
  ucbs_t_decl(bs, 128);
  ucbs_init_str(bs, "0");
  ucbs_setall(bs);
  assert(ucbs_all(bs) == true);
  ucbs_reset(bs, 0);
  assert(ucbs_test(bs, 0) == false);
  ucbs_set(bs, 0);
  assert(ucbs_all(bs) == true);
  for (size_t i = 0; i < 15; i += 2)
    ucbs_flip(bs, i);
  ucbs_t_decl_ptr(bs_flip, 128);
  ucbs_init_str(*bs_flip, "0101010101010101");
  ucbs_flipall(bs);
  assert(ucbs_equals(bs, *bs_flip));
  free(bs_flip);
  ucbs_resetall(bs);
  assert(ucbs_none(bs) == true);
}

static void test_bitset_ops(void) {
  ucbs_t_decl(bs1, 128);
  ucbs_init(bs1, ULONG_MAX);
  assert(ULONG_MAX == ucbs_to_ulong(bs1));
  ucbs_resetall(bs1);
  ucbs_init(bs1, ULONG_LONG_MAX);
  assert(ULONG_LONG_MAX == ucbs_to_ullong(bs1));

  ucbs_t_decl(bs2, 128);
  ucbs_init_str(bs2, ULL_BIN_STR);
  char *bs1_str = ucbs_to_string(bs1);
  assert(!strcmp(ULL_BIN_STR, bs1_str));
  free(bs1_str);
}

int main(void) {
  UCTEST_PRINT_THEAD();
  UCTEST_RUN("storage size", test_size());
  UCTEST_RUN("constructors", test_init());
  UCTEST_RUN("bit access", test_bit_access());
  UCTEST_RUN("bit operations", test_bit_ops());
  UCTEST_RUN("bitset operations", test_bitset_ops());
  return 0;
}
