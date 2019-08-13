#include "doddict.h"
#include <stdio.h>
#include <time.h>

static void test_doddict(void) {
  int v;
  char k[6] = {0};
  doddict_t *d = doddict_new(char[6], int, 3);
  srand(time(NULL));
  for (int j = 0; j < 5; ++j) {
    v = rand();
    sprintf(k, "key %d", j + 1);
    doddict_push_back(d, &k, &v);
    printf("(%s, %d)\n", k, v);
  }
  printf("----------\n");
  size_t dl = doddict_size(d);
  for (size_t i = 0; i < dl; ++i) {
    doddict_back_read(d, &k, &v);
    doddict_pop_back(d);
    printf("(%s, %d)\n", k, v);
  }
  printf("Size: %lu\n", doddict_size(d));
  doddict_free(d);
}

int main(void) {
  clock_t begin = clock();
  test_doddict();
  clock_t end = clock();
  double time_spent = (double) (end - begin) / CLOCKS_PER_SEC;
  printf("Took %f secs...\n", time_spent);
  return 0;
}
