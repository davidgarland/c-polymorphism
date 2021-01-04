#include <stdio.h>

#define T int
  #include "arr.h"
#undef T

void foo(void) {
  arr_int xs;
  arr_int_alloc(&xs, 1);
  arr_int_snoc(&xs, 1);
  arr_int_snoc(&xs, 2);
  for (int i = 0; i < xs.len; i++)
    printf("%i\n", xs.data[i]);
  arr_int_free(&xs);
}
