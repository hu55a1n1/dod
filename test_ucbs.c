#include <assert.h>
#include <stdio.h>
#include "ucbs.h"

int main(void) {
  ucbs_t(16) bs9 = {.bytes = {0}};
  ucbs_init(bs9, 10);
  ucbs_print(bs9);
  printf("\n");
  ucbs_t(16) bs8 = {.bytes = {0}};
  ucbs_init_str(bs8, "00001000");
  ucbs_print(bs8);
  return 0;
}
