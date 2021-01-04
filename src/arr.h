#include <stdlib.h>

#include "macro.h"

typedef struct {
  size_t len;
  size_t cap;
  T *data;
} C2(arr, T);

void C3(arr, T, alloc)(C2(arr, T) *, size_t);
void C3(arr, T, free)(C2(arr, T) *);

void C3(arr, T, snoc)(C2(arr, T) *, T);
