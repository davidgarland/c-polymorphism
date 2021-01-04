#ifndef MACRO_H
#define MACRO_H

#define C2_(A, B) A##_##B
#define C2(A, B) C2_(A, B)

#define C3_(A, B, C) A##_##B##_##C
#define C3(A, B, C) C3_(A, B, C)

#endif /* MACRO_H */
