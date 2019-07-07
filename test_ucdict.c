#include "ucdict.h"
#include <stdio.h>
#include <time.h>

static void test_ucdict() {
  int v;
  char k[6] = {0};
  ucdict_t d;
  ucdict_init(&d, char[6], int, 5);
  srand(time(NULL));
  for (int j = 0; j < 5; ++j) {
    v = rand();
    sprintf(k, "key %d", j + 1);
    ucdict_push_back(&d, &k, &v);
    printf("(%s, %d)\n", k, v);
  }
  printf("----------\n");
  size_t dl = ucdict_size(&d);
  for (size_t i = 0; i < dl; ++i) {
    ucdict_back(&d, &k, &v);
    ucdict_pop(&d);
    printf("(%s, %d)\n", k, v);
  }
  printf("Size: %lu\n", ucdict_size(&d));
  ucdict_free(&d);
}

int main() {
  clock_t begin = clock();
  test_ucdict();
  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Took %f secs...\n", time_spent);
  return 0;
}
