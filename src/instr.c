#include "../headers/instr.h"
void destroyInstr(Instr *ins) {
    destroyState(&ins->state);
    destroyEmit(&ins->emit);
}
