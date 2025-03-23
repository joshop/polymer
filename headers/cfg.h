#include "instr.h"
#include "vector.h"
DEFINE_VECTOR(Instr)
typedef struct BBlock {
    Vector(Instr) code;
    BBlock *next;
    BBlock *alt;
    Instr jump;
    Expr takeAlt;
} BBlock;
DEFINE_VECTOR(BBlock)
typedef struct Function {
    Vector(BBlock) blocks;
} Function;
