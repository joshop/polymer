#pragma once
#include "disasm.h"
#include "emit.h"
#include "state.h"
#include "vector.h"
typedef struct Instr {
    uint32_t addr;
    InsDecode decode;
    State state;
    Emit emit;
} Instr;
DEFINE_VECTOR(Instr)
void destroyInstr(Instr *ins);
