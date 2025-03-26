#pragma once
#include "instr.h"
#include "vector.h"
/* A basic block of instructions:
 *  start is the state before any instructions are executed,
 *  code is the set of instructions in this block,
 *  next is the next basic block, or NULL,
 *  alt is the basic block that may be jumped to instead, or NULL,
 *  jump is the instruction that decides between next and alt,
 *  takeAlt is an expression describing whether to go to alt.
 */
typedef struct BBlock {
    State start;
    Vector(Instr) code;
    struct BBlock *next;
    struct BBlock *alt;
    Instr jump;
    Expr *takeAlt;
} BBlock;
DEFINE_VECTOR(BBlock)
/* A function consisting of several basic blocks:
 *  blocks are the basic blocks making up this function.
 */
typedef struct Function {
    Vector(BBlock) blocks;
} Function;
DEFINE_VECTOR(Function)
// Destroys a function.
void destroyFunction(Function *func);
// Destroys a basic block.
void destroyBBlock(BBlock *blk);
// Extracts basic blocks from an array starting at ins into a vector blocks.
size_t createBlocks(Vector(BBlock) *blocks, Instr *ins);
// Propagates the state from blk->start into each instruction in blk->code.
void propagateBlock(BBlock *blk);
// Creates an expression representing whether a given jump is taken in the context of state.
Expr *calcTakeAlt(State *state, InsOpcode jmp);
