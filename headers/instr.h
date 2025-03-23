#pragma once
#include "disasm.h"
#include "emit.h"
#include "state.h"
typedef struct Instr {
    uint32_t addr;
    InsDecode decode;
    State state;
    Emit emit;
} Instr;
