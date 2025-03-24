#pragma once
#include <stdlib.h>
/* Reference-counting information for an object.
 *  refcnt is the current reference count,
 *  destroy is the function to be called when the count hits zero.
 */
typedef struct Object {
    size_t refcnt;
    void (*destroy)(struct Object*);
} Object;

// Allocates memory for an object with size size.
Object* createObject(size_t size);
// Increments the reference count of obj.
void addRef(Object* obj);
// Decrements the reference count of obj, possibly freeing and destroying it.
void release(Object* obj);

#define CREATE(t) (t*)createObject(sizeof(t))
#define ADDREF(x) addRef((Object*)(x))
#define RELEASE(x) release((Object*)(x))
