#include "disasm.h"
#include "instr.h"
#include "vector.h"
#include "cfg.h"
// Disassembles and decodes block_size bytes from code located at start_addr into decodes.
void disassembleBlock(Vector(Instr) *decodes, uint8_t *code, uint32_t start_addr, uint32_t block_size);
// Extracts functions and their basic blocks from the instruction vector decodes into funcs.
void extractFunctions(Vector(Instr) *decodes, Vector(Function) *funcs);
