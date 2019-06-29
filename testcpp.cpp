#include "cvec.h"
#define LOGARITHMIC_GROWTH
#include "cvector.h"
#include <cstdio>
#include <random>
#include <vector>

#define CVEC_MAX 100000

typedef struct {
  int x;
  int y;
} point_t;

static void test_cvec() {
  point_t p;
  vec v;
  vec_init(&v, point_t, 1024);
  srand(time(NULL));
  for (int j = 0; j < CVEC_MAX; ++j) {
    p.x = std::rand();
    p.y = std::rand();
    vec_push_back(&v, &p);
  }
  size_t vl = v.l;
  for (size_t i = 0; i < vl; ++i) {
    //    vec_back(&v, &p);
    //    //    printf("x = %d, y = %d\n", p.x, p.y);
    //    vec_pop(&v);
    vec_erase(&v, 0);
  }
  if (vec_empty(&v))
    printf("empty\n");
  vec_free(&v);
}

static void test_cvector() {
  point_t *v = NULL;
  point_t p;
  srand(time(NULL));
  for (int j = 0; j < CVEC_MAX; ++j) {
    p.x = std::rand();
    p.y = std::rand();
    vector_push_back(v, p);
  }
  size_t vl = vector_size(v);
  for (size_t i = 0; i < vl; ++i) {
    //    vec_back(&v, &p);
    //    //    printf("x = %d, y = %d\n", p.x, p.y);
    //    vec_pop(&v);
    vector_erase(v, 0);
  }
  if (vector_empty(v))
    printf("empty\n");
  vector_free(v);
}

static void test_veccpp() {
  point_t p;
  std::vector<point_t> v;
  srand(time(NULL));
  for (int j = 0; j < CVEC_MAX; ++j) {
    p.x = std::rand();
    p.y = std::rand();
    v.push_back(p);
  }
  size_t vl = v.size();
  for (size_t i = 0; i < vl; ++i) {
    //    auto &p = v.back();
    //    (void)p;
    //    //    printf("x = %d, y = %d\n", p.x, p.y);
    //    v.pop_back();
    v.erase(v.begin());
  }

  if (v.empty())
    printf("empty\n");
}

int main() {
  clock_t begin = clock();
  test_veccpp();
  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("std::vector = %f\n", time_spent);

  begin = clock();
  test_cvec();
  end = clock();
  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("cvec = %f\n", time_spent);

  begin = clock();
  test_cvector();
  end = clock();
  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("cvector = %f\n", time_spent);
  return 0;
}
