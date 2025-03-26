#include "../headers/state_propfuncs.h"
#include "../headers/state.h"
#include "../headers/expr.h"
//#include <stdio.h>
void propMOV(Expr* src, Expr* op2, Instr* ins) {
    stateStore(&ins->state, ins->decode.dst, src, &ins->emit);
}
void propADD(Expr* src, Expr* op2, Instr* ins) {
    stateStore(&ins->state, ins->decode.dst,
               createExprBin(src, bADD, op2), &ins->emit);
}
void propSUB(Expr* src, Expr* op2, Instr* ins) {
    stateStore(&ins->state, ins->decode.dst,
               createExprBin(src, bSUB, op2), &ins->emit);
}
void propAND(Expr* src, Expr* op2, Instr* ins) {
    stateStore(&ins->state, ins->decode.dst,
               createExprBin(src, bAND, op2), &ins->emit);
}
void propOR(Expr* src, Expr* op2, Instr* ins) {
    stateStore(&ins->state, ins->decode.dst,
               createExprBin(src, bOR, op2), &ins->emit);
}
void propXOR(Expr* src, Expr* op2, Instr* ins) {
    stateStore(&ins->state, ins->decode.dst,
               createExprBin(src, bXOR, op2), &ins->emit);
}
void propSHL(Expr* src, Expr* op2, Instr* ins) {
    stateStore(&ins->state, ins->decode.dst,
               createExprBin(src, bSHL, op2), &ins->emit);
}
void propSHR(Expr* src, Expr* op2, Instr* ins) {
    stateStore(&ins->state, ins->decode.dst,
               createExprBin(src, bSHR, op2), &ins->emit);
}
void propLEA(Expr* src, Expr* op2, Instr* ins) {
    stateStore(&ins->state, ins->decode.dst,
               calcEffAddress(src), &ins->emit);
}
void propPUSH(Expr* src, Expr* op2, Instr* ins) {
    VectorPushBlank(ins->state.stack);
    ADDREF(src);
    VectorLast(ins->state.stack) = src;
}
void propPOP(Expr* src, Expr* op2, Instr* ins) {
    Expr *top;
    VectorPop(ins->state.stack, top);
    RELEASE(top);
    stateStore(&ins->state, ins->decode.dst, top, &ins->emit);
}
void propINC(Expr* src, Expr* op2, Instr* ins) {
    stateStore(&ins->state, ins->decode.dst,
               createExprBin(op2, bADD, createAtomConst(1)), &ins->emit);
}
void propDEC(Expr* src, Expr* op2, Instr* ins) {
    stateStore(&ins->state, ins->decode.dst,
               createExprBin(op2, bSUB, createAtomConst(1)), &ins->emit);
}
void propCMP(Expr* src, Expr* op2, Instr* ins) {
    ins->state.flagtype = CMPFLAGS;
    ins->state.flagsleft = op2;
    ins->state.flagsright = src;
}
void propTEST(Expr* src, Expr* op2, Instr* ins) {
    ins->state.flagtype = TESTFLAGS;
    ins->state.flagsleft = op2;
    ins->state.flagsright = src;
}
