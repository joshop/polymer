#include <stdio.h>
#include <stdint.h>
#include "../headers/decomp.h"
#include "../headers/cfg.h"

int main() {
    //uint8_t code[] = {0x8B, 0x46, 0x02, 0x8B, 0x5E, 0x04, 0x03, 0x07, 0xC3, 0x00, 0x00, 0x00, 0x00};
    uint8_t code[] = {0x8B, 0x76, 0x02, 0x8B, 0x5E, 0x04, 0x8D, 0x40, 0x06, 0x89, 0x46, 0x04, 0xC3, 0x00, 0x00, 0x00, 0x00};
    size_t code_len = 13;
    Vector(Instr) decodes;
    VectorNew(decodes);
    disassembleBlock(&decodes, code, 0x100, code_len);
    Vector(Function) functions;
    VectorNew(functions);
    extractFunctions(&decodes, &functions);
    BBlock *blk = &VectorFirst(VectorFirst(functions).blocks);
    propagateBlock(blk);
    //char buf[1024];
    //exprToStr(buf, VectorLast(blk->code).state.regs[ax]);
    //printf("ax = %s\n", buf);
    VectorEachPtr(functions, destroyFunction);
    VectorDelete(functions);
    VectorDelete(decodes);
}
