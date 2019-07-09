#include "ucvec.h"
#include <assert.h>
#include <stdio.h>

typedef struct {
  int x;
  int y;
} struct_t;

#define test_ucvec_type(type, ...)                                             \
  do {                                                                         \
    type arr[] = {__VA_ARGS__};                                                \
    const size_t sz = (sizeof(arr) / sizeof(type));                            \
    size_t i = 0;                                                              \
    type j = {0};                                                              \
    ucvec_t *v = ucvec_new(type, sz);                                          \
    for (i = 0; i < sz; i++)                                                   \
      ucvec_push_back(v, arr + i);                                             \
    for (i = 0; i < sz; i++) {                                                 \
      ucvec_back(v, &j);                                                       \
      ucvec_pop(v);                                                            \
      assert(!memcmp(&j, &arr[sz - i - 1], sizeof(j)));                        \
    }                                                                          \
    ucvec_free(v);                                                             \
  } while (0)

static void test_ucvec_types(void) {
  test_ucvec_type(int, 1, 2, 3, 4, 5);
  test_ucvec_type(float, 1.0, 2.0, 3.0, 4.0, 5.0);
  test_ucvec_type(struct_t, {.x = 1, .y = 2}, {.x = 3, .y = 4},
                  {.x = 5, .y = 6}, {.x = 7, .y = 8}, {.x = 9, .y = 10});
  test_ucvec_type(char *, "str1", "str2", "str3", "str4", "str5");
  typedef char str5[5];
  test_ucvec_type(str5, "str1", "str2", "str3", "str4", "str5");
}

int main(void) {
  test_ucvec_types();
  return 0;
}
