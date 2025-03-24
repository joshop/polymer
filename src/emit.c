#include "../headers/emit.h"
void destroyEmit(Emit *emit) {
    if (emit->dest != NULL) {
        RELEASE(emit->dest);
        RELEASE(emit->src);
    }
}
