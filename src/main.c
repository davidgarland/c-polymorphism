#include <stdio.h>

extern void foo(void);

#define T int
  #include "arr.h"
#undef T

#define T float
  #include "arr.h"
#undef T

int main() {
  arr_int xs;
  arr_int_alloc(&xs, 1);
  arr_int_snoc(&xs, 5);
  arr_int_snoc(&xs, 3);
  arr_int_snoc(&xs, 7);
  for (int i = 0; i < xs.len; i++)
    printf("%i\n", xs.data[i]);
  arr_int_free(&xs);

  arr_float ys;
  arr_float_alloc(&ys, 1);
  arr_float_snoc(&ys, 5.6);
  arr_float_snoc(&ys, 3.7);
  arr_float_snoc(&ys, 7.25);
  for (int i = 0; i < ys.len; i++)
    printf("%f\n", ys.data[i]);
  arr_float_free(&ys);

  foo();
  
  return 0;
}

