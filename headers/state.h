#pragma once
#include "disasm.h"
#include "expr.h"
#include "vector.h"
DEFINE_VECTOR(Expr)
typedef struct State {
    Expr *regs[NUM_REGS];
    // segregs, and flags
    Vector(Expr) stack;
} State;
