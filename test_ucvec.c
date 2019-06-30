#include "ucvec.h"
#include <stdio.h>
#include <time.h>

typedef struct {
  int x;
  int y;
} point_t;

static void test_ucvec() {
  point_t p;
  ucvec_t v;
  ucvec_init(&v, point_t, 1024);
  srand(time(NULL));
  for (int j = 0; j < 100000000; ++j) {
    p.x = rand();
    p.y = rand();
    ucvec_push_back(&v, &p);
  }
  size_t vl = v.l;
  for (size_t i = 0; i < vl; ++i) {
    ucvec_pop(&v);
  }
  ucvec_free(&v);
}

int main() {
  clock_t begin = clock();
  test_ucvec();
  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Took %f secs...\n", time_spent);
  return 0;
}
