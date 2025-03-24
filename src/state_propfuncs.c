#include "../headers/state_propfuncs.h"
#include "../headers/state.h"
#include "../headers/expr.h"
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
