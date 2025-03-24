#include "../headers/cfg.h"

typedef BBlock** BBlockPlace;
DEFINE_VECTOR(BBlockPlace)


void destroyFunction(Function *func) {
    VectorEachPtr(func->blocks, destroyBBlock);
    VectorDelete(func->blocks);
}
void destroyBBlock(BBlock *blk) {
    VectorEachPtr(blk->code, destroyInstr);
    VectorDelete(blk->code);
    destroyState(&blk->start);
    destroyExpr((Object*)&blk->takeAlt);
}

size_t createBlocks(Vector(BBlock) *blocks, Instr *ins) {
    BBlock *cur_block = NULL;
    size_t num_instr = 0;
    BBlockPlace next_place = NULL;
    Vector(uint32_t) alt_addresses;
    Vector(BBlockPlace) alt_places;
    VectorNew(alt_addresses);
    VectorNew(alt_places);
    bool_t keep_going = true;
    while (keep_going) {
        if (cur_block == NULL) {
            VectorPushBlank(*blocks);
            cur_block = &VectorLast(*blocks);
            VectorNew(cur_block->code);
        }
        if (ins->decode.flags & (IFLAGS_JUMP | IFLAGS_NONEXT)) {
            if (ins->decode.flags & IFLAGS_JUMP) {
                VectorPush(alt_addresses, ins->decode.dst.im);
                VectorPush(alt_places, &cur_block->alt);
                cur_block->jump = *ins;
            } else {
                cur_block->alt = NULL;
            }
            if (!(ins->decode.flags & IFLAGS_NONEXT)) {
                next_place = &cur_block->next;
            } else {
                VectorPush(cur_block->code, *ins);
                cur_block->next = NULL;
                uint32_t next_address = (ins + 1)->addr;
                VectorContains(alt_addresses, next_address, &keep_going);
            }

        } else {
            VectorPush(cur_block->code, *ins);
        }
        if (next_place != NULL) {
            *next_place = cur_block;
            next_place = NULL;
        }
        ins++;
        num_instr++;
    }
    // TODO: make this part not quadratic time
    // even though one of them is literally sorted alr
    for (size_t i = 0; i < alt_addresses.size; i++) {
        for (size_t j = 0; j < blocks->size; i++) {
            if (blocks->data[j].code.data->addr == alt_addresses.data[i]) {
                *alt_places.data[i] = &blocks->data[j];
            }
        }
    }
    VectorDelete(alt_addresses);
    VectorDelete(alt_places);
    return num_instr;
}
void propagateBlock(BBlock *blk) {
    State *last = &blk->start;
    for (size_t i = 0; i < blk->code.size; i++) {
        propagateState(last, &blk->code.data[i]);
        last = &blk->code.data[i].state;
    }
}
