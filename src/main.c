#include <stdio.h>
#include "../headers/disasm.h"

int main() {
    FILE *com = fopen("basic.com", "r");
    uint8_t buf[512];
    fread(buf, 512, 1, com);
    uint8_t *ins = buf;
    uint32_t address = 0x100;
    char insbuf[128];
    while (ins < buf + 512) {
        InsDecode dec = disassemble(&ins, &address);
        decodedToStr(insbuf, &dec);
        puts(insbuf);
    }
}
