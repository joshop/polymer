#pragma once
#include "disasm.h"
#include "expr.h"
#include "emit.h"
#include "vector.h"
DEFINE_VECTOR(Expr)
typedef struct Instr Instr;
typedef Expr* Expr_;
DEFINE_VECTOR(Expr_)
/* The state of register allocation at a given point.
 *  regs[r] holds the expression located in register r,
 *  stack.data[i] holds the expression on the ith stack position.
 */
typedef enum FlagType { NOFLAGS, CMPFLAGS, TESTFLAGS } FlagType;
typedef struct State {
    Expr *regs[NUM_REGS];
    // TODO: segregs, and flags
    Vector(Expr_) stack;
    FlagType flagtype;
    Expr *flagsleft;
    Expr *flagsright;
} State;
// Computes the effects of the instruction ins on prev_state, and updates ins->state.
void propagateState(State *prev_state, Instr *ins);
// Destroys a state.
void destroyState(State *old);
// Modifies state and emit due to the effects of storing expr in the operand op.
void stateStore(State *state, InsOperand op, Expr *expr, Emit *emit);
// Represents a source operand as an expression, in the context of a state from_state.
Expr *expressSource(State *from_state, InsOperand op);
