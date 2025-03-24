#pragma once
#include "disasm.h"
#include "expr.h"
#include "emit.h"
#include "vector.h"
DEFINE_VECTOR(Expr)
typedef struct Instr Instr;
typedef Expr* Expr_;
DEFINE_VECTOR(Expr_)
typedef struct State {
    Expr *regs[NUM_REGS];
    // TODO: segregs, and flags
    Vector(Expr_) stack;
} State;
void propagateState(State *prev_state, Instr *ins);
void destroyState(State *old);
void stateStore(State *state, InsOperand op, Expr *expr, Emit *emit);
