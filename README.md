# C Polymorphism

This repo demonstrates how you would do performant specialized polymorphism
in C, rather than doing polymorphism via `void*` or having a bunch of inlined
functions from single-header libraries.

# Overview

First there is a file `macro.h` that defines some simple macros:

```C
// macro.h
#ifndef MACRO_H
#define MACRO_H

#define C2_(A, B) A##_##B
#define C2(A, B) C2_(A, B)

#define C3_(A, B, C) A##_##B##_##C
#define C3(A, B, C) C3_(A, B, C)

#endif /* MACRO_H */
```

What these do is give you an underscore-separated concatenation of the
identifiers passed in as arguments. For instance `C2(hello, world)` would yield
the identifier `hello_world`.

The "second layer" of macro without the underscore is to make sure that the
preprocessor expands any macros passed in as arguments. With this power we can
create snippets of code that could have parts of identifiers swapped out.

For instance, let's make a header for a polymorphic array type, carefully
excluding the header guard so it can be included multiple times:

```C
// arr.h
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
```

Here `T` specifies a type name, and we use that both in the definition of the
structure as well as to define its name. Some function signatures are also
defined below: allocation, freeing, and "snoc" ("cons" but the other way around;
the more popular name is "push_back".)

The implementation is trivial (ignoring things like properly handling allocation
failure, asserts to check for NULL arguments or zero capacity, et cetera):

```C
// arr.c
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
```

Now in an actual program, we can include `arr.h` for any type we want like so:

```C
#define T int
  #include "arr.h"
#undef T
```

And then in a single compilation unit in the project, you just have to do the
following:

```C
#define T int
  #include "arr.c"
#undef T
```

And that will generate the single specialization for that type used across the
whole project. No template name mangling BS, no extreme code size bloat from
inlining, no indirection and tagging costs from using void pointers. It just
works.

In the attached program, `defs.c` is the single translation unit that defines
`arr` for the different types, and then those are used both in `main.c` and
`foo.c`.

To compile the program, do `cc foo.c main.c defs.c`.

# Gotchas

- This won't work for, say, arrays of non-typedef'd pointers, because you can't use `*` in an identifier. Same goes for array types, un-typedef'd structs, or types with qualifiers. Typedefs and `stdint.h` are your friend.
- Things like `arr_bool` will not work with `stdbool.h` as expected, because `bool` is a macro expanding to `_Bool` instead of a typedef for some reason. Instead you get `arr__Bool`. gross.
- This is a rare problem, but libraries based on other libraries using this method should not define ["orphan instances"](https://wiki.haskell.org/Orphan_instance); that is, if you are defining an instance (using the `.c` file) of a polymorphic structure that your library did not create for a type that you did not create, you run the risk of another library defining that same instance and then being incompatible with yours. If you either own the polymorphic structure or the type it's instantiated with, there is no risk of this happening. If you run into this problem, I would recommend either having flags to disable the orphan instances individually or asking the end user to do the instantiation of the orphan instance for you; if all libraries do this, there is no conflict.
