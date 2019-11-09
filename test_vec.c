#include "dodtest.h"
#include "dodvec.h"
#include <assert.h>
#include <stdio.h>
#include <time.h>

typedef struct {
  int x;
  int y;
} struct_t;

#define test_dodvec_push_pop_generic(type, ...)                                \
  do {                                                                         \
    type arr[] = {__VA_ARGS__};                                                \
    const size_t sz = (sizeof(arr) / sizeof(type));                            \
    size_t i = 0;                                                              \
    dodvec_t_decl_ptr(v, type, sz);                                            \
    for (i = 0; i < sz; i++)                                                   \
      dodvec_push_back(v, arr + i);                                            \
    for (i = 0; i < sz; i++) {                                                 \
      dodvec_pop_back(v);                                                      \
    }                                                                          \
    dodvec_free(v);                                                            \
  } while (0)

#define test_dodvec_erase_generic(type, ...)                                   \
  do {                                                                         \
    type arr[] = {__VA_ARGS__};                                                \
    const size_t sz = (sizeof(arr) / sizeof(type));                            \
    size_t i = 0;                                                              \
    dodvec_t_decl_ptr(v, type, sz);                                            \
    for (i = 0; i < sz; i++)                                                   \
      dodvec_push_back(v, arr + i);                                            \
    assert(!dodvec_erase(v, 0));                                               \
    assert(!memcmp(dodvec_at(v, 0), &arr[1], sizeof(type)));                   \
    dodvec_free(v);                                                            \
  } while (0)

static void test_dodvec_push_pop(void) {
  DODTEST_RUN("push pop for int",
              test_dodvec_push_pop_generic(int, 1, 2, 3, 4, 5));
  DODTEST_RUN("push pop for float",
              test_dodvec_push_pop_generic(float, 1.0, 2.0, 3.0, 4.0, 5.0));
  DODTEST_RUN("push pop for struct_t",
              test_dodvec_push_pop_generic(
                  struct_t, {.x = 1, .y = 2}, {.x = 3, .y = 4},
                  {.x = 5, .y = 6}, {.x = 7, .y = 8}, {.x = 9, .y = 10}));
  DODTEST_RUN("push pop for char *",
              test_dodvec_push_pop_generic(char *, "str1", "str2", "str3",
                                           "str4", "str5"));
  typedef char str5[5];
  DODTEST_RUN("push pop for str5",
              test_dodvec_push_pop_generic(str5, "str1", "str2", "str3", "str4",
                                           "str5"));
}

static void test_dodvec_erase(void) {
  DODTEST_RUN("erase for int", test_dodvec_erase_generic(int, 1, 2, 3, 4, 5));
  DODTEST_RUN("erase for float",
              test_dodvec_erase_generic(float, 1.0, 2.0, 3.0, 4.0, 5.0));
  DODTEST_RUN("erase for struct_t",
              test_dodvec_erase_generic(struct_t, {.x = 1, .y = 2},
                                        {.x = 3, .y = 4}, {.x = 5, .y = 6},
                                        {.x = 7, .y = 8}, {.x = 9, .y = 10}));
  DODTEST_RUN("erase for char *",
              test_dodvec_erase_generic(char *, "str1", "str2", "str3", "str4",
                                        "str5"));
  typedef char str5[5];
  DODTEST_RUN(
      "erase for str5",
      test_dodvec_erase_generic(str5, "str1", "str2", "str3", "str4", "str5"));
}

static void test_dodvec_access(void) {
  dodvec_t_decl_ptr(v, int, 5);
  for (int i = 0; i < 5; ++i)
    dodvec_push_back(v, &i);

  int *j = dodvec_at(v, 2);
  assert(*j == 2);
  j = dodvec_front(v);
  assert(*j == 0);
  j = dodvec_back(v);
  assert(*j == 4);
  dodvec_free(v);
}

static void test_dodvec_shrink_to_fit(void) {
  dodvec_t_decl_ptr(v, int, 5);
  for (int i = 0; i < 5; ++i)
    dodvec_push_back(v, &i);

  int j = 6;
  dodvec_push_back(v, &j);
  dodvec_shrink_to_fit(v);
  assert(dodvec_capacity(v) == 6);

  // edge case
  dodvec_clear(v);
  assert(dodvec_size(v) == 0);
  dodvec_shrink_to_fit(v);
  assert(dodvec_capacity(v) == 0);
  dodvec_push_back(v, &j);
  assert(dodvec_size(v) == 1);
  dodvec_free(v);
}

static void test_dodvec_reserve(void) {
  dodvec_t_decl_ptr(v, int, 5);
  dodvec_reserve(v, 12);
  assert(dodvec_capacity(v) >= 12);
  dodvec_reserve(v, 4);
  assert(dodvec_capacity(v) >= 4);
  dodvec_reserve(v, 120);
  assert(dodvec_capacity(v) >= 120);
  dodvec_free(v);
}

static void test_dodvec_resize(void) {
  dodvec_t_decl_ptr(v, int, 5);
  size_t i;
  for (i = 0; i < 10; ++i)
    dodvec_push_back(v, &i);
  dodvec_resize(v, 5, NULL);
  assert(dodvec_size(v) == 5);
  int j = 100;
  dodvec_resize(v, 8, &j);
  assert(dodvec_size(v) == 8);
  for (i = 5; i < dodvec_size(v); ++i)
    assert(*dodvec_at(v, i) == 100);
  dodvec_resize(v, 12, NULL);
  assert(dodvec_size(v) == 12);
  for (i = 8; i < dodvec_size(v); ++i)
    assert(*dodvec_at(v, i) == 0);
  dodvec_free(v);
}

static void test_dodvec_assign(void) {
  dodvec_t_decl_ptr(v1, int, 5);
  for (int i = 0; i < 5; ++i)
    dodvec_push_back(v1, &i);
  dodvec_t_decl_ptr(v2, int, 5);
  for (int j = 3; j != 0; --j)
    dodvec_push_back(v2, &j);
  dodvec_assign_range(v1, dodvec_at(v2, 0), dodvec_at(v2, 2));
  assert(dodvec_size(v1) == 2);
  assert(*dodvec_at(v1, 0) == *dodvec_at(v2, 0));
  assert(*dodvec_at(v1, 1) == *dodvec_at(v2, 1));
  dodvec_free(v1);
  dodvec_free(v2);
  dodvec_t_decl_ptr(v3, int, 0);
  int val = 100;
  dodvec_assign_fill(v3, 10, &val);
  assert(dodvec_size(v3) == 10);
  for (size_t k = 0; k < dodvec_size(v3); ++k)
    assert(*dodvec_at(v3, k) == 100);
  dodvec_free(v3);
  dodvec_t_decl_ptr(v4, int, 10);
  int vals[] = {5, 4, 3, 2, 1};
  size_t valsz = (sizeof(vals) / sizeof(*vals));
  dodvec_assign_range(v4, vals, vals + valsz);
  assert(dodvec_size(v4) == valsz);
  for (size_t l = 0; l < dodvec_size(v4); ++l)
    assert(*dodvec_at(v4, l) == vals[l]);
  dodvec_free(v4);
}

static void test_dodvec_insert(void) {
  dodvec_t_decl_ptr(v, int, 3);
  int i = 100;
  dodvec_assign_fill(v, 3, &i);
  i = 200;
  dodvec_insert(v, dodvec_front(v), &i);

  i = 300;
  dodvec_insert(v, dodvec_front(v), &i);
  dodvec_insert(v, dodvec_front(v), &i);

  dodvec_t_decl_ptr(v1, int, 2);
  i = 400;
  dodvec_assign_fill(v1, 2, &i);
  dodvec_insert_range(v, dodvec_at(v, 2), dodvec_begin(v1), dodvec_end(v1));
  dodvec_free(v1);

  int vals[] = {501, 502, 503};
  dodvec_insert_range(v, dodvec_front(v), vals, vals + 3);

  int test_vals[] = {501, 502, 503, 300, 300, 400, 400, 200, 100, 100, 100};
  assert(!memcmp(dodvec_data(v), test_vals, sizeof(test_vals)));
  dodvec_free(v);
}

static void test_dodvec_swap(void) {
  int vals1[] = {5, 4, 3, 2, 1};
  size_t valsz1 = (sizeof(vals1) / sizeof(*vals1));
  dodvec_t_decl_ptr(v1, int, valsz1);
  dodvec_assign_range(v1, vals1, vals1 + valsz1);
  int vals2[] = {1, 2, 3, 4, 5};
  size_t valsz2 = (sizeof(vals2) / sizeof(*vals2));
  dodvec_t_decl_ptr(v2, int, valsz2);
  dodvec_assign_range(v2, vals2, vals2 + valsz2);
  dodvec_swap(v1, v2);
  for (size_t l = 0; l < dodvec_size(v1); ++l)
    assert(*dodvec_at(v1, l) == vals2[l]);
  for (size_t l = 0; l < dodvec_size(v2); ++l)
    assert(*dodvec_at(v2, l) == vals1[l]);
  dodvec_free(v2);
  dodvec_free(v1);
}

int main(void) {
  DODTEST_PRINT_THEAD();
  test_dodvec_push_pop();
  test_dodvec_erase();
  DODTEST_RUN("element access", test_dodvec_access());
  DODTEST_RUN("shrink to fit", test_dodvec_shrink_to_fit());
  DODTEST_RUN("reserve", test_dodvec_reserve());
  DODTEST_RUN("resize", test_dodvec_resize());
  DODTEST_RUN("assign", test_dodvec_assign());
  DODTEST_RUN("insert", test_dodvec_insert());
  DODTEST_RUN("swap", test_dodvec_swap());
  return 0;
}
