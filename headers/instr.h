#pragma once
#include "disasm.h"
#include "emit.h"
#include "state.h"
#include "vector.h"
/* One decompiled instruction with metadata.
 *  addr is the address of the instruction,
 *  decode is the decoded machine code instruction,
 *  state is the state of registers after execution of this instruction,
 *  emit describes any emitted stores.
 */
typedef struct Instr {
    uint32_t addr;
    InsDecode decode;
    State state;
    Emit emit;
} Instr;
DEFINE_VECTOR(Instr)
// Destroys an instruction.
void destroyInstr(Instr *ins);
