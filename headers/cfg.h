#pragma once
#include "instr.h"
#include "vector.h"

typedef struct BBlock {
    State start;
    Vector(Instr) code;
    struct BBlock *next;
    struct BBlock *alt;
    Instr jump;
    Expr takeAlt;
} BBlock;
DEFINE_VECTOR(BBlock)
typedef struct Function {
    Vector(BBlock) blocks;
} Function;
DEFINE_VECTOR(Function)
void destroyFunction(Function *func);
void destroyBBlock(BBlock *blk);
size_t createBlocks(Vector(BBlock) *blocks, Instr *ins);
void propagateBlock(BBlock *blk);
