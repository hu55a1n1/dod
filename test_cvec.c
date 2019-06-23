#include "cvec.h"
#include <stdio.h>

typedef struct {
  int x;
  int y;
} point_t;

int main() {
  point_t p1 = {.x = 10, .y = 11}, p2 = {.x = 12, .y = 13},
          p3 = {.x = 20, .y = 21}, p4 = {.x = 22, .y = 23};
  point_t p;
  vec v;
  vec_init(&v);
  vec_push_back(&v, &p1);
  vec_push_back(&v, &p2);
  vec_push_back(&v, &p3);
  vec_push_back(&v, &p4);

  size_t found = vec_find(&v, &p3);
  (found == v.l + 1) ? printf("Not found!\n")
                     : printf("Found at index %lu\n", found);

  size_t vl = vec_size(&v);
  for (int i = 0; i < vl; ++i) {
    vec_pop(&v, &p);
    printf("(x, y) === (%d, %d)\n", p.x, p.y);
  }
  return 0;
}