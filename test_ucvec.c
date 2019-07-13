#include "ucvec.h"
#include <assert.h>
#include <stdio.h>
#include <time.h>

typedef struct {
  int x;
  int y;
} struct_t;

#define RUN_TEST(msg, t)                                                       \
  do {                                                                         \
    printf("| %30s | ", msg);                                                  \
    t;                                                                         \
    printf("OK |\n");                                                          \
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
      ucvec_pop(v);                                                            \
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
  RUN_TEST("test push pop for int",
           test_ucvec_push_pop_generic(int, 1, 2, 3, 4, 5));
  RUN_TEST("test push pop for float",
           test_ucvec_push_pop_generic(float, 1.0, 2.0, 3.0, 4.0, 5.0));
  RUN_TEST("test push pop for struct_t",
           test_ucvec_push_pop_generic(struct_t, {.x = 1, .y = 2},
                                       {.x = 3, .y = 4}, {.x = 5, .y = 6},
                                       {.x = 7, .y = 8}, {.x = 9, .y = 10}));
  RUN_TEST("test push pop for char *",
           test_ucvec_push_pop_generic(char *, "str1", "str2", "str3", "str4",
                                       "str5"));
  typedef char str5[5];
  RUN_TEST("test push pop for str5",
           test_ucvec_push_pop_generic(str5, "str1", "str2", "str3", "str4",
                                       "str5"));
}

static void test_ucvec_erase(void) {
  RUN_TEST("test erase for int", test_ucvec_erase_generic(int, 1, 2, 3, 4, 5));
  RUN_TEST("test erase for float",
           test_ucvec_erase_generic(float, 1.0, 2.0, 3.0, 4.0, 5.0));
  RUN_TEST("test erase for struct_t",
           test_ucvec_erase_generic(struct_t, {.x = 1, .y = 2},
                                    {.x = 3, .y = 4}, {.x = 5, .y = 6},
                                    {.x = 7, .y = 8}, {.x = 9, .y = 10}));
  RUN_TEST(
      "test erase for char *",
      test_ucvec_erase_generic(char *, "str1", "str2", "str3", "str4", "str5"));
  typedef char str5[5];
  RUN_TEST(
      "test erase for str5",
      test_ucvec_erase_generic(str5, "str1", "str2", "str3", "str4", "str5"));
}

int main(void) {
  test_ucvec_push_pop();
  test_ucvec_erase();
  return 0;
}
