#include <stdlib.h>
typedef struct Object {
    size_t refcnt;
    void (*destroy)(struct Object*);
} Object;

Object* createObject(size_t size) {
    Object* object = (Object*) calloc(1, size);

    if (object != NULL) {
        object->refcnt = 0;
    }

    return object;
}

void addRef(Object* obj) {
    obj->refcnt++;
}

void release(Object* obj) {

    if (obj->refcnt == 0 || --obj->refcnt == 0) {
        if (obj->destroy != NULL)
            obj->destroy(obj);
        free(obj);
    }
}
