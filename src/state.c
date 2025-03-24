#include <stdio.h>
#include "../headers/state.h"
#include "../headers/instr.h"
#include "../headers/state_propfuncs.h"
Expr *expressSource(State *from_state, InsOperand op) {
    switch(op.type) {
        case UNUSED:
            return createExprUninit();
        case REG:
            return from_state->regs[op.r];
        case IMM:
            return createAtomConst(op.im);
        case ADDRESS:
            if (op.r == na && op.r2 == na) {
                return createAtomGlobal(op.im);
            } else if (op.r2 == na) {
                if ((op.r == sp || op.r == bp) && from_state->regs[op.r]->typ == UNINIT) {
                    return createAtomLocal(op.im);
                }
                if (op.im == 0) {
                    return createExprUn(from_state->regs[op.r], uDEREF);
                } else {
                    Expr *offset = createAtomConst(op.im);
                    return createExprUn(
                        createExprBin(from_state->regs[op.r], bADD, offset), uDEREF);
                }
            } else {
                if (op.im == 0) {
                    return createExprUn(
                        createExprBin(from_state->regs[op.r], bADD, from_state->regs[op.r2]), uDEREF);
                } else {
                    Expr *offset = createAtomConst(op.im);
                    return createExprUn(
                        createExprBin(createExprBin(from_state->regs[op.r], bADD, from_state->regs[op.r2]), bADD, offset), uDEREF);
                }
            }
        default:
            return createExprUninit();
    }
}
extern const char* REGNAMES[];
#define DECOMPDEBUG
void stateStore(State *state, InsOperand op, Expr *expr, Emit *emit) {
    #ifdef DECOMPDEBUG
    char buf[256];
    #endif
    switch(op.type) {
        case REG:
#ifdef DECOMPDEBUG
            exprToStr(buf, expr);
            printf("%14s <= %s", REGNAMES[op.r], buf);
#endif
            ADDREF(expr);
            RELEASE(state->regs[op.r]);
            state->regs[op.r] = expr;
            break;
        case IMM:
            assert(0);
        case ADDRESS:
            ;
            Expr *tmp = expressSource(state, op);
            emit->dest = tmp;
            ADDREF(tmp);
            ADDREF(tmp);
            emit->src = expr;
            ADDREF(expr);
#ifdef DECOMPDEBUG
            exprToStr(buf, tmp);
            printf("%14s <=", buf);
            exprToStr(buf, expr);
            printf(" %s", buf);
#endif
            RELEASE(tmp);
            break;
        default:
            return;
    }
}

extern const char* OPCODENAMES[];



void (*const PROPFUNCS[NUM_OPCODES])(Expr*, Expr*, Instr*) = {
    [MOV] = propMOV,
    [ADD] = propADD,
    [SUB] = propSUB,
    [AND] = propAND,
    [OR] = propOR,
    [XOR] = propXOR,
    [SHL] = propSHL,
    [SHR] = propSHR,
    [LEA] = propLEA
};

void propagateState(State *prev_state, Instr *ins) {
#ifdef DECOMPDEBUG
    char buf[30];
    decodedToStr(buf, &ins->decode);
    printf("0x%8x: %-30s |", ins->addr, buf);
#endif
    ins->state = *prev_state;
    VectorClone(prev_state->stack, ins->state.stack);
    for (uint8_t i = 0; i <NUM_REGS; i++) {
        ADDREF(ins->state.regs[i]);
    }
    for (size_t i = 0; i < ins->state.stack.size; i++) {
        ADDREF(ins->state.stack.data[i]);
    }
    Expr *src = expressSource(prev_state, ins->decode.src);
    Expr *op2 = expressSource(prev_state, ins->decode.op2);
    ADDREF(src);
    ADDREF(op2);
    if (PROPFUNCS[ins->decode.type] != NULL) {
        PROPFUNCS[ins->decode.type](src, op2, ins);
    }
    RELEASE(src);
    RELEASE(op2);
    printf("\n");

}
void destroyState(State *old) {
    for (uint8_t i = 0; i < NUM_REGS; i++) {
        if (old->regs[i] == NULL) continue;
        int j = old->regs[i]->obj.refcnt;
        RELEASE(old->regs[i]);
    }
    for (size_t i = 0; i < old->stack.size; i++) {
        RELEASE(old->stack.data[i]);
    }
    VectorDestroy(old->stack);
}
