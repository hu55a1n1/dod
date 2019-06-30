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

  cvec_t v;
  cvec_init(&v, point_t, 128);
  cvec_push_back(&v, &p1);
  cvec_push_back(&v, &p2);
  cvec_push_back(&v, &p3);
  cvec_push_back(&v, &p4);

  size_t found = cvec_find(&v, &p3);
  (found == v.l + 1) ? printf("Not found!\n")
                     : printf("Found at index %lu\n", found);
  cvec_erase(&v, found);

  size_t vl = cvec_size(&v);
  for (int i = 0; i < vl; ++i) {
    cvec_back(&v, &p);
    printf("(x, y) === (%d, %d)\n", p.x, p.y);
    cvec_pop(&v);
  }
  //  if (cvec_empty(&v)) printf("Empty\n");
  return 0;
}