#ifndef _CORE_H
#define _CORE_H

#include<stdlib.h>
#include<stdio.h>
#include<assert.h>
#include<stdint.h>
#include<stdbool.h>
#include<string.h>
#include<memory.h>

#define COMMA ,

#define DEFINE_VECTOR_OF(type) typedef struct { type* arr; size_t size, cap; }
#define VECTOR_INIT(vector) do { (vector).arr = malloc(sizeof((vector).arr[0])); (vector).size = 0; (vector).cap = 1; } while(0)
#define VECTOR_EMPTY(vector) ((vector).size == 0)
#define VECTOR_FREE(vector) free((vector).arr)
#define VECTOR_PUSH(vector, value) do { if((vector).size == (vector).cap){ (vector).cap *= 2; (vector).arr = realloc((vector).arr, sizeof((vector).arr[0]) * (vector).cap); } (vector).arr[(vector).size++] = value; } while(0)
#define VECTOR_POP(vector) ((vector).arr[--(vector).size])
#define VECTOR_POPF(vector) ((vector).arr[0]); memmove(&(vector).arr[0], &(vector).arr[1], --(vector).size * sizeof((vector).arr[0]))
#define VECTOR_REMOVE(vector, index) do { (vector).arr[index] = (vector).arr[(vector).size - 1]; (vector).size--; } while(0)
#define VECTOR_COPY(dst, src) do { (dst).cap = (src).cap; (dst).size = (src).size; (dst).arr = malloc((dst).cap * sizeof((dst).arr[0])); memcpy((dst).arr, (src).arr, (dst).size * sizeof((dst).arr[0])); } while(0)

typedef uint8_t byte;

#endif /* _CORE_H */