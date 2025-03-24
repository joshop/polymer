#include "disasm.h"
#include "instr.h"
#include "vector.h"
#include "cfg.h"
void disassembleBlock(Vector(Instr) *decodes, uint8_t *code, uint32_t start_addr, uint32_t block_size);
void extractFunctions(Vector(Instr) *decodes, Vector(Function) *funcs);
