#pragma once
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include "object.h"
#define DEFAULT_CAPACITY 8
#define DEFINE_VECTOR(type) struct Vector_##type { \
    type* data; \
    size_t size; \
    size_t capacity; \
};

#define Vector(type) struct Vector_##type
#define VectorNew(vec) { (vec).data = calloc(DEFAULT_CAPACITY, sizeof(*(vec).data)); (vec).size = 0; (vec).capacity = DEFAULT_CAPACITY; }
#define VectorDelete(vec) free((vec).data);
#define VectorDestroy(vec) { for (size_t i = 0; i < (vec).size; i++) { RELEASE(&(vec).data[i]); } free((vec).data); }
#define VectorResize(vec, newcap) { (vec).data = realloc((vec).data, newcap*sizeof(*(vec).data)); (vec).capacity = newcap; }
#define VectorChkSize(vec) { if ((vec).size < (vec).capacity / 4 && (vec).capacity > DEFAULT_CAPACITY) VectorResize(vec, (vec).capacity / 2); \
                             if ((vec).size > (vec).capacity / 2) VectorResize(vec, (vec).capacity * 2); }
#define VectorPush(vec, el) { (vec).data[(vec).size++] = el; VectorChkSize(vec) }
#define VectorPushBlank(vec) { (vec).size++; VectorChkSize(vec) }
#define VectorPop(vec, dst) { assert((vec).size); dst = (vec).data[--(vec).size]; VectorChkSize(vec) }
#define VectorRemove(vec, idx) { assert(idx < (vec).size); (vec).size--; memmove((vec).data + idx, (vec).data + idx + 1, ((vec).size - idx) * sizeof(*(vec).data)); VectorChkSize(vec); }
#define VectorFirst(vec) (vec).data[0]
#define VectorLast(vec) (vec).data[(vec).size - 1]
#define VectorContains(vec, val, chk) { *chk = false; for (size_t i = 0; i < (vec).size; i++) { if ((vec).data[i] == val) { *chk = true; break; }} }
#define VectorEach(vec, func) { for (size_t i = 0; i < (vec).size; i++) { func((vec).data[i]);} }
#define VectorEachPtr(vec, func) { for (size_t i = 0; i < (vec).size; i++) { func(&(vec).data[i]);} }
#define VectorClone(vec1, vec2) {(vec2).data = calloc((vec1).capacity, sizeof(*(vec1).data)); (vec2).size = (vec1).size; (vec2).capacity = (vec1).capacity; \
                                    memcpy((vec2).data, (vec1).data, (vec1).capacity * sizeof(*(vec1).data)); }

DEFINE_VECTOR(uint32_t)
