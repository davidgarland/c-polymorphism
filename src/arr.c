#include "arr.h"

void C3(arr, T, alloc)(C2(arr, T) *a, size_t cap) {
  a->data = malloc(cap * sizeof(T));
  a->len = 0;
  a->cap = cap;
}

void C3(arr, T, free)(C2(arr, T) *a) {
  free(a->data);
}

void C3(arr, T, snoc)(C2(arr, T) *a, T x) {
  if (a->len == a->cap) {
    a->cap += a->cap;
    a->data = realloc(a->data, a->cap * sizeof(T));
  }
  a->data[a->len++] = x;
}
