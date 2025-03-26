#include "expr.h"
#include "instr.h"
/* All functions here propagate states for individual opcodes.
 */
void propMOV(Expr* src, Expr* op2, Instr* ins);
void propADD(Expr* src, Expr* op2, Instr* ins);
void propSUB(Expr* src, Expr* op2, Instr* ins);
void propAND(Expr* src, Expr* op2, Instr* ins);
void propOR(Expr* src, Expr* op2, Instr* ins);
void propXOR(Expr* src, Expr* op2, Instr* ins);
void propSHL(Expr* src, Expr* op2, Instr* ins);
void propSHR(Expr* src, Expr* op2, Instr* ins);
void propLEA(Expr* src, Expr* op2, Instr* ins);
void propPUSH(Expr*src, Expr* op2, Instr* ins);
void propPOP(Expr*src, Expr* op2, Instr* ins);
void propINC(Expr*src, Expr* op2, Instr* ins);
void propDEC(Expr*src, Expr* op2, Instr* ins);
void propCMP(Expr*src, Expr* op2, Instr* ins);
void propTEST(Expr*src, Expr* op2, Instr* ins);
