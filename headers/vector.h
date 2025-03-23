#include <stdlib.h>
#include <assert.h>
#define DEFAULT_CAPACITY 8
#define DEFINE_VECTOR(type) struct Vector_##type { \
    type* data; \
    size_t size; \
    size_t capacity; \
};

#define Vector(type) struct Vector_##type
#define VectorNew(vec) { vec.data = calloc(DEFAULT_CAPACITY, sizeof(*vec.data)); vec.size = 0; vec.capacity = DEFAULT_CAPACITY; }
#define VectorDelete(vec) free(vec.data);
#define VectorResize(vec, newcap) { vec.data = realloc(vec.data, newcap*sizeof(*vec.data)); vec.capacity = newcap; }
#define VectorChkSize(vec) { if (vec.size < vec.capacity / 4 && vec.capacity > DEFAULT_CAPACITY) VectorResize(vec, vec.capacity / 2); \
                             if (vec.size > vec.capacity / 2) VectorResize(vec, vec.capacity * 2); }
#define VectorPush(vec, el) { vec.data[vec.size++] = el; VectorChkSize(vec) }
#define VectorPop(vec, dst) { assert(vec.size); dst = vec.data[--vec.size]; VectorChkSize(vec) }
