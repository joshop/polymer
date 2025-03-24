#include "expr.h"
#include "instr.h"
void propMOV(Expr* src, Expr* op2, Instr* ins);
void propADD(Expr* src, Expr* op2, Instr* ins);
void propSUB(Expr* src, Expr* op2, Instr* ins);
void propAND(Expr* src, Expr* op2, Instr* ins);
void propOR(Expr* src, Expr* op2, Instr* ins);
void propXOR(Expr* src, Expr* op2, Instr* ins);
void propSHL(Expr* src, Expr* op2, Instr* ins);
void propSHR(Expr* src, Expr* op2, Instr* ins);
void propLEA(Expr* src, Expr* op2, Instr* ins);
