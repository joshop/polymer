#pragma once
#include "expr.h"
/* Information about emitted decompilation from an instruction:
 *  dest is where this line stores data,
 *  src is what this line stores into dest.
 */
typedef struct Emit {
    Expr *dest;
    Expr *src;
} Emit;
// Destroys an emit.
void destroyEmit(Emit *emit);
