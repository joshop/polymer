#pragma once
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include "object.h"
#define DEFAULT_CAPACITY 8
// Macro to define a vector struct of a given type.
// Use typedef to remove * from type names before defining vectors.
#define DEFINE_VECTOR(type) struct Vector_##type { \
    type* data; \
    size_t size; \
    size_t capacity; \
};

// A vector with elements of type type.
#define Vector(type) struct Vector_##type
// Initializes a vector vec.
#define VectorNew(vec) { (vec).data = calloc(DEFAULT_CAPACITY, sizeof(*(vec).data)); (vec).size = 0; (vec).capacity = DEFAULT_CAPACITY; }
// Frees a vector vec.
#define VectorDelete(vec) free((vec).data);
// Releases each element of vec, then frees it.
#define VectorDestroy(vec) { for (size_t i = 0; i < (vec).size; i++) { RELEASE(&(vec).data[i]); } free((vec).data); }
// Reallocates the vector to have a capacity of newcap.
#define VectorResize(vec, newcap) { (vec).data = realloc((vec).data, newcap*sizeof(*(vec).data)); (vec).capacity = newcap; }
// Resizes the vector to be between 1/4 and 1/2 full.
#define VectorChkSize(vec) { if ((vec).size < (vec).capacity / 4 && (vec).capacity > DEFAULT_CAPACITY) VectorResize(vec, (vec).capacity / 2); \
                             if ((vec).size > (vec).capacity / 2) VectorResize(vec, (vec).capacity * 2); }
// Pushes an element el to the end of vec.
#define VectorPush(vec, el) { (vec).data[(vec).size++] = el; VectorChkSize(vec) }
// Pushes a zero-filled element to the end of vec.
#define VectorPushBlank(vec) { (vec).size++; memset(&(vec).data[(vec).size-1], 0, sizeof(*(vec).data)); VectorChkSize(vec) }
// Pops an element from the end of vec into dst.
#define VectorPop(vec, dst) { assert((vec).size); dst = (vec).data[--(vec).size]; VectorChkSize(vec) }
// Removes element idx from vec.
#define VectorRemove(vec, idx) { assert(idx < (vec).size); (vec).size--; memmove((vec).data + idx, (vec).data + idx + 1, ((vec).size - idx) * sizeof(*(vec).data)); VectorChkSize(vec); }
// The first element of vec.
#define VectorFirst(vec) (vec).data[0]
// The last element of vec.
#define VectorLast(vec) (vec).data[(vec).size - 1]
// Sets chk based on whether vec contains val.
#define VectorContains(vec, val, chk) { *chk = false; for (size_t i = 0; i < (vec).size; i++) { if ((vec).data[i] == val) { *chk = true; break; }} }
// Runs func on each element of vec.
#define VectorEach(vec, func) { for (size_t i = 0; i < (vec).size; i++) { func((vec).data[i]);} }
// Runs func on a pointer to each element of vec.
#define VectorEachPtr(vec, func) { for (size_t i = 0; i < (vec).size; i++) { func(&(vec).data[i]);} }
// Clones a vector and copies over its data from vec1 to vec2.
#define VectorClone(vec1, vec2) {(vec2).data = calloc((vec1).capacity, sizeof(*(vec1).data)); (vec2).size = (vec1).size; (vec2).capacity = (vec1).capacity; \
                                    memcpy((vec2).data, (vec1).data, (vec1).capacity * sizeof(*(vec1).data)); }

DEFINE_VECTOR(uint32_t)
