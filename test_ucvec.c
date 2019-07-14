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

static void test_ucvec_access() {
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

static void test_ucvec_shrink_to_fit() {
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

static void test_ucvec_reserve() {
  ucvec_t *v = ucvec_new(int, 5);
  ucvec_reserve(v, 12);
  assert(ucvec_capacity(v) >= 12);
  ucvec_reserve(v, 4);
  assert(ucvec_capacity(v) >= 4);
  ucvec_reserve(v, 120);
  assert(ucvec_capacity(v) >= 120);
  ucvec_free(v);
}
  ucvec_free(v);
}

int main(void) {
  TEST_PRINT_THEAD();
  test_ucvec_push_pop();
  test_ucvec_erase();
  TEST_RUN("element access", test_ucvec_access());
  TEST_RUN("shrink to fit", test_ucvec_shrink_to_fit());
  TEST_RUN("reserve", test_ucvec_reserve());
  return 0;
}
