#pragma once
#include <stdlib.h>
typedef struct Object {
    size_t refcnt;
    void (*destroy)(struct Object*);
} Object;

Object* createObject(size_t size);
void addRef(Object* obj);
void release(Object* obj);
#define CREATE(t) (t*)createObject(sizeof(t))
#define ADDREF(x) addRef((Object*)x)
#define RELEASE(x) release((Object*)x)
