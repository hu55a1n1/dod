#include "cvec.h"
#include <cstdio>
#include <random>
#include <vector>

#define CVEC_MAX 10000000

typedef struct {
  int x;
  int y;
} point_t;

void test_cvec() {
  point_t p;
  vec v;
  vec_init(&v);
  srand(time(NULL));
  for (int j = 0; j < CVEC_MAX; ++j) {
    p.x = std::rand();
    p.y = std::rand();
    vec_push_back(&v, &p);
  }
  size_t vl = v.l;
  for (int i = 0; i < vl; ++i) {
    vec_back(&v, &p);
    //    printf("x = %d, y = %d\n", p.x, p.y);
    vec_pop(&v);
  }
}

void test_veccpp() {
  point_t p;
  std::vector<point_t> v;
  srand(time(NULL));
  for (int j = 0; j < CVEC_MAX; ++j) {
    p.x = std::rand();
    p.y = std::rand();
    v.push_back(p);
  }
  size_t vl = v.size();
  for (int i = 0; i < vl; ++i) {
    auto &p = v.back();
    //    printf("x = %d, y = %d\n", p.x, p.y);
    v.pop_back();
  }
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
  return 0;
}
