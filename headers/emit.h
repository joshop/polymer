#pragma once
#include "expr.h"
typedef struct Emit {
    Expr *dest;
    Expr *src;
} Emit;
void destroyEmit(Emit *emit);
