#include "../headers/decomp.h"
#include "../headers/instr.h"
#include "../headers/cfg.h"

void disassembleBlock(Vector(Instr) *decodes, uint8_t *code, uint32_t start_addr, uint32_t block_size) {
    //uint32_t next_component = 1;
    //bool_t last_newnext = false;
    //Vector(uint32_t) futureJumps;
    //VectorNew(futureJumps);
    uint32_t curaddr = start_addr;
    for (uint8_t *curcode = code; curcode < code + block_size;) {
        Instr next;
        memset(&next, 0, sizeof(Instr));
        next.addr = curaddr;
        InsDecode dec = disassembleSingle(&curcode, &curaddr);
        next.decode = dec;
        /*if (dec.flags | IFLAGS_JUMP) {
            uint32_t tgt = dec.dst.im;
            if (tgt > curaddr) {
                VectorPush(futureJumps, tgt);
            }
        }
        last_newnext = !(dec.flags | IFLAGS_NONEXT);
        if (last_newnext) {
            bool_t new_component = true;
            for (size_t i = 0; i < futureJumps.size; i++) {
                if (futureJumps.data[i] == curaddr) {
                    new_component = false;
                    VectorRemove(futureJumps, i);
                    break;
                }
            }
            if (new_component) next_component++;
        }
        dec.component = next_component;*/
        VectorPush(*decodes, next);
    }
    //VectorDelete(futureJumps);
}
void extractFunctions(Vector(Instr) *decodes, Vector(Function) *funcs) {
    size_t processed = 0;
    while (processed < decodes->size) {
        VectorPushBlank(*funcs);
        VectorNew(VectorLast(*funcs).blocks);
        processed += createBlocks(&VectorLast(*funcs).blocks, decodes->data + processed);
        State *init_state = &VectorFirst(VectorLast(*funcs).blocks).start;
        for (uint8_t i = 0; i < NUM_REGS; i++) {
            init_state->regs[i] = createExprUninit();
            ADDREF(init_state->regs[i]);
        }
        VectorNew(init_state->stack);
    }
}
