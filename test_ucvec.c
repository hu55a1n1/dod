#include "ucvec.h"
#include <assert.h>
#include <stdio.h>
#include <time.h>

typedef struct {
  int x;
  int y;
} struct_t;

#define TEST_PRINT_THEAD() fprintf(stderr, "| %25s | %6s |\n", "Test", "Result")
#define TEST_RUN(msg, t)                                                       \
  do {                                                                         \
    fprintf(stderr, "| %25s | ", msg);                                         \
    t;                                                                         \
    fprintf(stderr, "%6s |\n", "OK");                                          \
  } while (0)

#define test_ucvec_push_pop_generic(type, ...)                                 \
  do {                                                                         \
    type arr[] = {__VA_ARGS__};                                                \
    const size_t sz = (sizeof(arr) / sizeof(type));                            \
    size_t i = 0;                                                              \
    ucvec_t *v = ucvec_new(type, sz);                                          \
    for (i = 0; i < sz; i++)                                                   \
      ucvec_push_back(v, arr + i);                                             \
    for (i = 0; i < sz; i++) {                                                 \
      assert(!memcmp(ucvec_back(v), &arr[sz - i - 1], sizeof(type)));          \
      ucvec_pop_back(v);                                                       \
    }                                                                          \
    ucvec_free(v);                                                             \
  } while (0)

#define test_ucvec_erase_generic(type, ...)                                    \
  do {                                                                         \
    type arr[] = {__VA_ARGS__};                                                \
    const size_t sz = (sizeof(arr) / sizeof(type));                            \
    size_t i = 0;                                                              \
    ucvec_t *v = ucvec_new(type, sz);                                          \
    for (i = 0; i < sz; i++)                                                   \
      ucvec_push_back(v, arr + i);                                             \
    assert(!ucvec_erase(v, 0));                                                \
    assert(!memcmp(ucvec_at(v, 0), &arr[1], sizeof(type)));                    \
    ucvec_free(v);                                                             \
  } while (0)

static void test_ucvec_push_pop(void) {
  TEST_RUN("push pop for int", test_ucvec_push_pop_generic(int, 1, 2, 3, 4, 5));
  TEST_RUN("push pop for float",
           test_ucvec_push_pop_generic(float, 1.0, 2.0, 3.0, 4.0, 5.0));
  TEST_RUN("push pop for struct_t",
           test_ucvec_push_pop_generic(struct_t, {.x = 1, .y = 2},
                                       {.x = 3, .y = 4}, {.x = 5, .y = 6},
                                       {.x = 7, .y = 8}, {.x = 9, .y = 10}));
  TEST_RUN("push pop for char *",
           test_ucvec_push_pop_generic(char *, "str1", "str2", "str3", "str4",
                                       "str5"));
  typedef char str5[5];
  TEST_RUN("push pop for str5",
           test_ucvec_push_pop_generic(str5, "str1", "str2", "str3", "str4",
                                       "str5"));
}

static void test_ucvec_erase(void) {
  TEST_RUN("erase for int", test_ucvec_erase_generic(int, 1, 2, 3, 4, 5));
  TEST_RUN("erase for float",
           test_ucvec_erase_generic(float, 1.0, 2.0, 3.0, 4.0, 5.0));
  TEST_RUN("erase for struct_t",
           test_ucvec_erase_generic(struct_t, {.x = 1, .y = 2},
                                    {.x = 3, .y = 4}, {.x = 5, .y = 6},
                                    {.x = 7, .y = 8}, {.x = 9, .y = 10}));
  TEST_RUN(
      "erase for char *",
      test_ucvec_erase_generic(char *, "str1", "str2", "str3", "str4", "str5"));
  typedef char str5[5];
  TEST_RUN("erase for str5", test_ucvec_erase_generic(str5, "str1", "str2",
                                                      "str3", "str4", "str5"));
}

static void test_ucvec_access(void) {
  ucvec_t *v = ucvec_new(int, 5);
  for (int i = 0; i < 5; ++i)
    ucvec_push_back(v, &i);

  int *j = (int *)ucvec_at(v, 2);
  assert(*j == 2);
  j = (int *)ucvec_front(v);
  assert(*j == 0);
  j = (int *)ucvec_back(v);
  assert(*j == 4);
  ucvec_free(v);
}

static void test_ucvec_shrink_to_fit(void) {
  ucvec_t *v = ucvec_new(int, 5);
  for (int i = 0; i < 5; ++i)
    ucvec_push_back(v, &i);

  int j = 6;
  ucvec_push_back(v, &j);
  ucvec_shrink_to_fit(v);
  assert(ucvec_capacity(v) == 6);

  // edge case
  ucvec_clear(v);
  assert(ucvec_size(v) == 0);
  ucvec_shrink_to_fit(v);
  assert(ucvec_capacity(v) == 0);
  ucvec_push_back(v, &j);
  assert(ucvec_size(v) == 1);
  ucvec_free(v);
}

static void test_ucvec_reserve(void) {
  ucvec_t *v = ucvec_new(int, 5);
  ucvec_reserve(v, 12);
  assert(ucvec_capacity(v) >= 12);
  ucvec_reserve(v, 4);
  assert(ucvec_capacity(v) >= 4);
  ucvec_reserve(v, 120);
  assert(ucvec_capacity(v) >= 120);
  ucvec_free(v);
}

static void test_ucvec_resize(void) {
  ucvec_t *v = ucvec_new(int, 10);
  size_t i;
  for (i = 0; i < 10; ++i)
    ucvec_push_back(v, &i);
  ucvec_resize(v, 5, NULL);
  assert(ucvec_size(v) == 5);
  int j = 100;
  ucvec_resize(v, 8, &j);
  assert(ucvec_size(v) == 8);
  for (i = 5; i < ucvec_size(v); ++i)
    assert(*ucvec_at(v, i) == 100);
  ucvec_resize(v, 12, NULL);
  assert(ucvec_size(v) == 12);
  for (i = 8; i < ucvec_size(v); ++i)
    assert(*ucvec_at(v, i) == 0);
  ucvec_free(v);
}

static void test_ucvec_assign(void) {
  ucvec_t *v1 = ucvec_new(int, 5);
  for (int i = 0; i < 5; ++i)
    ucvec_push_back(v1, &i);
  ucvec_t *v2 = ucvec_new(int, 3);
  for (int j = 3; j != 0; --j)
    ucvec_push_back(v2, &j);
  ucvec_assign_range(v1, ucvec_at(v2, 0), ucvec_at(v2, 2));
  assert(ucvec_size(v1) == 2);
  assert(*ucvec_at(v1, 0) == *ucvec_at(v2, 0));
  assert(*ucvec_at(v1, 1) == *ucvec_at(v2, 1));
  ucvec_free(v1);
  ucvec_free(v2);
  ucvec_t *v3 = ucvec_new(int, 0);
  int val = 100;
  ucvec_assign_fill(v3, 10, &val);
  assert(ucvec_size(v3) == 10);
  for (size_t k = 0; k < ucvec_size(v3); ++k)
    assert(*ucvec_at(v3, k) == 100);
  ucvec_free(v3);
  ucvec_t *v4 = ucvec_new(int, 10);
  int vals[] = {5, 4, 3, 2, 1};
  size_t valsz = (sizeof(vals) / sizeof(*vals));
  ucvec_assign_range(v4, vals, vals + valsz);
  assert(ucvec_size(v4) == valsz);
  for (size_t l = 0; l < ucvec_size(v4); ++l)
    assert(*ucvec_at(v4, l) == vals[l]);
  ucvec_free(v4);
}

static void test_ucvec_insert(void) {
  ucvec_t *v = ucvec_new(int, 3);
  int i = 100;
  ucvec_assign_fill(v, 3, &i);

  i = 200;
  ucvec_insert(v, ucvec_front(v), &i);

  i = 300;
  ucvec_insert(v, ucvec_front(v), &i);
  ucvec_insert(v, ucvec_front(v), &i);

  ucvec_t *v1 = ucvec_new(int, 2);
  i = 400;
  ucvec_assign_fill(v1, 2, &i);
  ucvec_insert_range(v, ucvec_at(v, 2), ucvec_begin(v1), ucvec_end(v1));
  ucvec_free(v1);

  int vals[] = {501, 502, 503};
  ucvec_insert_range(v, ucvec_front(v), vals, vals + 3);

  int test_vals[] = {501, 502, 503, 300, 300, 400, 400, 200, 100, 100, 100};
  assert(!memcmp(ucvec_data(v), test_vals, sizeof(test_vals)));
  ucvec_free(v);
}

static void test_ucvec_swap(void) {
  int vals1[] = {5, 4, 3, 2, 1};
  size_t valsz1 = (sizeof(vals1) / sizeof(*vals1));
  ucvec_t *v1 = ucvec_new(int, valsz1);
  ucvec_assign_range(v1, vals1, vals1 + valsz1);
  int vals2[] = {1, 2, 3, 4, 5};
  size_t valsz2 = (sizeof(vals2) / sizeof(*vals2));
  ucvec_t *v2 = ucvec_new(int, valsz2);
  ucvec_assign_range(v2, vals2, vals2 + valsz2);
  ucvec_swap(v1, v2);
  for (size_t l = 0; l < ucvec_size(v1); ++l)
    assert(*ucvec_at(v1, l) == vals2[l]);
  for (size_t l = 0; l < ucvec_size(v2); ++l)
    assert(*ucvec_at(v2, l) == vals1[l]);
  ucvec_free(v2);
  ucvec_free(v1);
}

int main(void) {
  TEST_PRINT_THEAD();
  test_ucvec_push_pop();
  test_ucvec_erase();
  TEST_RUN("element access", test_ucvec_access());
  TEST_RUN("shrink to fit", test_ucvec_shrink_to_fit());
  TEST_RUN("reserve", test_ucvec_reserve());
  TEST_RUN("resize", test_ucvec_resize());
  TEST_RUN("assign", test_ucvec_assign());
  TEST_RUN("insert", test_ucvec_insert());
  TEST_RUN("swap", test_ucvec_swap());
  return 0;
}
