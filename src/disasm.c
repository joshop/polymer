#include "../headers/disasm.h"
#include <stdio.h>

typedef enum Bits { b8, b16, b32 } Bits;


const Reg MODRM_REGS8[] = {al, cl, dl, bl, ah, ch, dh, bh};
const Reg MODRM_REGS16[] = {ax, cx, dx, bx, sp, bp, si, di};
const Reg MODRM_RM1[] = {bx, bx, bp, bp, si, di, bp, bx};
const Reg MODRM_RM2[] = {si, di, si, di, na, na, na, na};
void modrmSib(uint8_t *code, Bits size, InsOperand *reg, InsOperand *rm) {
    // TODO: SIB byte not implemented
    uint8_t modrm = code[0];
    if (size == b8) {
        reg->r = MODRM_REGS8[(modrm & 0x38) >> 3];
    } else {
        reg->r = MODRM_REGS16[(modrm & 0x38) >> 3];
    }
    reg->type = REG;
    rm->type = ADDRESS;
    rm->r = MODRM_RM1[(modrm & 0x07)];
    rm->r2 = MODRM_RM2[(modrm & 0x07)];
    rm->im = 0;
    switch (modrm >> 6) {
        case 0x0:
            if (rm->r == bp && rm->r2 == na) rm->r = na;
            break;
        case 0x1:
            rm->im = *(int8_t*)(code + 1);
            break;
        case 0x2:
            rm->im = *(int16_t*)(code + 1);
            break;
        case 0x3:
            if (size == b8) {
                rm->r = MODRM_REGS8[(modrm & 0x07)];
            } else {
                rm->r = MODRM_REGS16[(modrm & 0x07)];
            }
            rm->type = REG;
            break;
    }
}

const char* REGNAMES[] = {"ax", "bx", "cx", "dx", "al", "bl", "cl", "dl", "ah", "bh", "ch", "dh", "si", "di", "bp", "sp"};
void operandToStr(char *buf, InsOperand *op) {
    if (op->type == UNUSED) {
        *buf = 0;
    } else if (op->type == REG) {
        sprintf(buf, "%s", REGNAMES[op->r]);
    } else if (op->type == REGHL) {
        sprintf(buf, "%s:%s", REGNAMES[op->r2], REGNAMES[op->r]);
    } else if (op->type == IMM) {
        sprintf(buf, "0x%x", op->im);
    } else if (op->type == ADDRESS) {
        if (op->r == na) {
            sprintf(buf, "[0x%x]", op->im);
        } else if (op->r2 == na) {
            sprintf(buf, "[%s+0x%x]", REGNAMES[op->r], op->im);
        } else {
            sprintf(buf, "[%s+%s+0x%x]", REGNAMES[op->r], REGNAMES[op->r2], op->im);
        }
    } else {
        sprintf(buf, "(unimpl)");
    }
}
typedef enum InsParamType { NONE, SAME, RM8, RM16, REG8, REG16, IMMU8, IMM8, IMM16, IMMU16, SREG,
                            REGAX, REGBX, REGCX, REGDX,
                            REGAL, REGBL, REGCL, REGDL,
                            REGAH, REGBH, REGCH, REGDH,
                            REGSI, REGDI, REGBP, REGSP,
                            REL8, REL16, ADDR16, TODO,
                            REGDS, REGES, REGSS, REGCS
} InsParamType;

typedef struct OpcodeTableEntry {
    InsOpcode op;
    InsParamType param1; // destination
    InsParamType param2; // 2nd operand
    InsParamType param3; // 1st operand - usually same as destination
    uint8_t size;
} OpcodeTableEntry;

const OpcodeTableEntry OPCODE_TABLE[256] = {
    // 0x00
    {ADD,   RM8,    REG8,   SAME,   2},
    {ADD,   RM16,   REG16,  SAME,   2},
    {ADD,   REG8,   RM8,    SAME,   2},
    {ADD,   REG16,  RM16,   SAME,   2},
    {ADD,   REGAL,  IMMU8,  SAME,   2},
    {ADD,   REGAX,  IMM16,  SAME,   3},
    {PUSH,  REGES,  NONE,   SAME,   1},
    {POP,   REGES,  NONE,   SAME,   1},
    {OR,    RM8,    REG8,   SAME,   2},
    {OR,    RM16,   REG16,  SAME,   2},
    {OR,    REG8,   RM8,    SAME,   2},
    {OR,    REG16,  RM16,   SAME,   2},
    {OR,    REGAL,  IMMU8,  SAME,   2},
    {OR,    REGAX,  IMM16,  SAME,   3},
    {PUSH,  REGCS,  NONE,   SAME,   1},
    {BADOPCODE},

    // 0x10
    {ADC,   RM8,    REG8,   SAME,   2},
    {ADC,   RM16,   REG16,  SAME,   2},
    {ADC,   REG8,   RM8,    SAME,   2},
    {ADC,   REG16,  RM16,   SAME,   2},
    {ADC,   REGAL,  IMMU8,  SAME,   2},
    {ADC,   REGAX,  IMM16,  SAME,   3},
    {PUSH,  REGSS,  NONE,   SAME,   1},
    {POP,   REGSS,  NONE,   SAME,   1},
    {SBB,   RM8,    REG8,   SAME,   2},
    {SBB,   RM16,   REG16,  SAME,   2},
    {SBB,   REG8,   RM8,    SAME,   2},
    {SBB,   REG16,  RM16,   SAME,   2},
    {SBB,   REGAL,  IMMU8,  SAME,   2},
    {SBB,   REGAX,  IMM16,  SAME,   3},
    {PUSH,  REGDS,  NONE,   SAME,   1},
    {POP,   REGDS,  NONE,   SAME,   1},

    // 0x20
    {AND,   RM8,    REG8,   SAME,   2},
    {AND,   RM16,   REG16,  SAME,   2},
    {AND,   REG8,   RM8,    SAME,   2},
    {AND,   REG16,  RM16,   SAME,   2},
    {AND,   REGAL,  IMMU8,  SAME,   2},
    {AND,   REGAX,  IMM16,  SAME,   3},
    {BADOPCODE},
    {DAA,   NONE,   NONE,   SAME,   1},
    {SUB,   RM8,    REG8,   SAME,   2},
    {SUB,   RM16,   REG16,  SAME,   2},
    {SUB,   REG8,   RM8,    SAME,   2},
    {SUB,   REG16,  RM16,   SAME,   2},
    {SUB,   REGAL,  IMMU8,  SAME,   2},
    {SUB,   REGAX,  IMM16,  SAME,   3},
    {DAS,   NONE,   NONE,   SAME,   1},
    {BADOPCODE},

    // 0x30
    {XOR,   RM8,    REG8,   SAME,   2},
    {XOR,   RM16,   REG16,  SAME,   2},
    {XOR,   REG8,   RM8,    SAME,   2},
    {XOR,   REG16,  RM16,   SAME,   2},
    {XOR,   REGAL,  IMMU8,  SAME,   2},
    {XOR,   REGAX,  IMM16,  SAME,   3},
    {BADOPCODE},
    {AAA,   NONE,   NONE,   SAME,   1},
    {CMP,   RM8,    REG8,   SAME,   2},
    {CMP,   RM16,   REG16,  SAME,   2},
    {CMP,   REG8,   RM8,    SAME,   2},
    {CMP,   REG16,  RM16,   SAME,   2},
    {CMP,   REGAL,  IMMU8,  SAME,   2},
    {CMP,   REGAX,  IMM16,  SAME,   3},
    {BADOPCODE},
    {AAS,   NONE,   NONE,   SAME,   1},

    // 0x40
    {INC,   REGAX,  NONE,   SAME,   1},
    {INC,   REGCX,  NONE,   SAME,   1},
    {INC,   REGDX,  NONE,   SAME,   1},
    {INC,   REGBX,  NONE,   SAME,   1},
    {INC,   REGSP,  NONE,   SAME,   1},
    {INC,   REGBP,  NONE,   SAME,   1},
    {INC,   REGSI,  NONE,   SAME,   1},
    {INC,   REGDI,  NONE,   SAME,   1},
    {DEC,   REGAX,  NONE,   SAME,   1},
    {DEC,   REGCX,  NONE,   SAME,   1},
    {DEC,   REGDX,  NONE,   SAME,   1},
    {DEC,   REGBX,  NONE,   SAME,   1},
    {DEC,   REGSP,  NONE,   SAME,   1},
    {DEC,   REGBP,  NONE,   SAME,   1},
    {DEC,   REGSI,  NONE,   SAME,   1},
    {DEC,   REGDI,  NONE,   SAME,   1},

    // 0x50
    {PUSH,  REGAX,  NONE,   SAME,   1},
    {PUSH,  REGCX,  NONE,   SAME,   1},
    {PUSH,  REGDX,  NONE,   SAME,   1},
    {PUSH,  REGBX,  NONE,   SAME,   1},
    {PUSH,  REGSP,  NONE,   SAME,   1},
    {PUSH,  REGBP,  NONE,   SAME,   1},
    {PUSH,  REGSI,  NONE,   SAME,   1},
    {PUSH,  REGDI,  NONE,   SAME,   1},
    {POP,   REGAX,  NONE,   SAME,   1},
    {POP,   REGCX,  NONE,   SAME,   1},
    {POP,   REGDX,  NONE,   SAME,   1},
    {POP,   REGBX,  NONE,   SAME,   1},
    {POP,   REGSP,  NONE,   SAME,   1},
    {POP,   REGBP,  NONE,   SAME,   1},
    {POP,   REGSI,  NONE,   SAME,   1},
    {POP,   REGDI,  NONE,   SAME,   1},

    // 0x60
    {PUSHA, NONE,   NONE,   SAME,   1},
    {POPA,  NONE,   NONE,   SAME,   1},
    {BADOPCODE},
    {BADOPCODE},
    {BADOPCODE},
    {BADOPCODE},
    {BADOPCODE},
    {BADOPCODE},
    {PUSH,  IMM16,  NONE,   SAME,   3},
    {IMUL,  REG16,  IMM16,  RM16,   4},
    {PUSH,  IMM8,   NONE,   SAME,   2},
    {IMUL,  REG16,  IMM8,   RM8,    3},
    {BADOPCODE},
    {BADOPCODE},
    {BADOPCODE},
    {BADOPCODE},

    // 0x70
    {JO,    REL8,   NONE,   SAME,   2},
    {JNO,   REL8,   NONE,   SAME,   2},
    {JB,    REL8,   NONE,   SAME,   2},
    {JAE,   REL8,   NONE,   SAME,   2},
    {JE,    REL8,   NONE,   SAME,   2},
    {JNE,   REL8,   NONE,   SAME,   2},
    {JBE,   REL8,   NONE,   SAME,   2},
    {JA,    REL8,   NONE,   SAME,   2},
    {JS,    REL8,   NONE,   SAME,   2},
    {JNS,   REL8,   NONE,   SAME,   2},
    {JP,    REL8,   NONE,   SAME,   2},
    {JNP,   REL8,   NONE,   SAME,   2},
    {JL,    REL8,   NONE,   SAME,   2},
    {JGE,   REL8,   NONE,   SAME,   2},
    {JLE,   REL8,   NONE,   SAME,   2},
    {JG,    REL8,   NONE,   SAME,   2},

    // 0x80
    {BADOPCODE},
    {BADOPCODE},
    {BADOPCODE},
    {BADOPCODE},
    {TEST,  RM8,    REG8,   SAME,   2},
    {TEST,  RM16,   REG16,  SAME,   2},
    {XCHG,  REG8,   RM8,    SAME,   2},
    {XCHG,  REG16,  RM16,   SAME,   2},
    {MOV,   RM8,    REG8,   SAME,   2},
    {MOV,   RM16,   REG16,  SAME,   2},
    {MOV,   REG8,   RM8,    SAME,   2},
    {MOV,   REG16,  RM16,   SAME,   2},
    {MOV,   RM16,   SREG,   SAME,   2},
    {LEA,   REG16,  RM16,   SAME,   2},
    {MOV,   SREG,   RM16,   SAME,   2},
    {POP,   RM16,   NONE,   SAME,   2},

    // 0x90
    {NOP,   NONE,   NONE,   SAME,   1},
    {XCHG,  REGAX,  REGCX,  SAME,   1},
    {XCHG,  REGAX,  REGDX,  SAME,   1},
    {XCHG,  REGAX,  REGBX,  SAME,   1},
    {XCHG,  REGAX,  REGSP,  SAME,   1},
    {XCHG,  REGAX,  REGBP,  SAME,   1},
    {XCHG,  REGAX,  REGSI,  SAME,   1},
    {XCHG,  REGAX,  REGDI,  SAME,   1},
    {BADOPCODE},
    {BADOPCODE},
    {CALL,  IMM16,  NONE,   SAME,   3},
    {BADOPCODE},
    {PUSHF, NONE,   NONE,   SAME,   1},
    {POPF,  NONE,   NONE,   SAME,   1},
    {SAHF,  NONE,   NONE,   SAME,   1},
    {LAHF,  NONE,   NONE,   SAME,   1},

    // 0xA0
    {MOV,   REGAL,  ADDR16, SAME,   2},
    {MOV,   REGAX,  ADDR16, SAME,   2},
    {MOV,   ADDR16, REGAL,  SAME,   2},
    {MOV,   ADDR16, REGAX,  SAME,   2},
    {MOVSB, NONE,   NONE,   SAME,   1},
    {MOVSW, NONE,   NONE,   SAME,   1},
    {CMPSB, NONE,   NONE,   SAME,   1},
    {CMPSW, NONE,   NONE,   SAME,   1},
    {TEST,  REGAL,  IMMU8,  SAME,   2},
    {TEST,  REGAX,  IMM16,  SAME,   3},
    {STOSB, NONE,   NONE,   SAME,   1},
    {STOSW, NONE,   NONE,   SAME,   1},
    {LODSB, NONE,   NONE,   SAME,   1},
    {LODSW, NONE,   NONE,   SAME,   1},
    {SCASB, NONE,   NONE,   SAME,   1},
    {SCASW, NONE,   NONE,   SAME,   1},

    // 0xB0
    {MOV,   REGAL, IMMU8,   SAME,   2},
    {MOV,   REGCL, IMMU8,   SAME,   2},
    {MOV,   REGDL, IMMU8,   SAME,   2},
    {MOV,   REGBL, IMMU8,   SAME,   2},
    {MOV,   REGAH, IMMU8,   SAME,   2},
    {MOV,   REGCH, IMMU8,   SAME,   2},
    {MOV,   REGDH, IMMU8,   SAME,   2},
    {MOV,   REGBH, IMMU8,   SAME,   2},
    {MOV,   REGAX, IMM16,   SAME,   3},
    {MOV,   REGCX, IMM16,   SAME,   3},
    {MOV,   REGDX, IMM16,   SAME,   3},
    {MOV,   REGBX, IMM16,   SAME,   3},
    {MOV,   REGSP, IMM16,   SAME,   3},
    {MOV,   REGBP, IMM16,   SAME,   3},
    {MOV,   REGSI, IMM16,   SAME,   3},
    {MOV,   REGDI, IMM16,   SAME,   3},

    // 0xC0
    {BADOPCODE},
    {BADOPCODE},
    {RET,   IMMU16,NONE,    SAME,   3},
    {RET,   NONE,  NONE,    SAME,   1},
    {LES,   REG16, RM16,    SAME,   2},
    {LDS,   REG16, RM16,    SAME,   2},
    {MOV,   RM8,   IMMU8,   SAME,   3},
    {MOV,   RM16,  IMMU16,  SAME,   4},
    {BADOPCODE},
    {BADOPCODE},
    {RETF,  IMMU16,NONE,    SAME,   3},
    {RETF,  NONE,  NONE,    SAME,   1},
    {INT3,  NONE,  NONE,    SAME,   1},
    {INT,   IMMU8, NONE,    SAME,   2},
    {INTO,  NONE,  NONE,    SAME,   1},
    {IRET,  NONE,  NONE,    SAME,   1},

    // 0xD0
    {BADOPCODE},
    {BADOPCODE},
    {BADOPCODE},
    {BADOPCODE},
    {AAM,   IMMU8,  NONE,   SAME,   2},
    {AAD,   IMMU8,  NONE,   SAME,   2},
    {BADOPCODE},
    {XLATB, NONE,   NONE,   SAME,   1},
    {BADOPCODE},
    {BADOPCODE},
    {BADOPCODE},
    {BADOPCODE},
    {BADOPCODE},
    {BADOPCODE},
    {BADOPCODE},
    {BADOPCODE},

    // 0xE0
    {LOOPNE,REL8,   NONE,   SAME,  2},
    {LOOPE, REL8,   NONE,   SAME,  2},
    {LOOP,  REL8,   NONE,   SAME,  2},
    {JCXZ,  REL8,   NONE,   SAME,  2},
    {IN,    REGAL,  IMMU8,  SAME,  2},
    {IN,    REGAX,  IMMU8,  SAME,  2},
    {OUT,   IMMU8,  REGAL,  SAME,  2},
    {OUT,   IMMU8,  REGAX,  SAME,  2},
    {CALL,  REL16,  NONE,   SAME,  3},
    {JMP,   REL16,  NONE,   SAME,  3},
    {JMP,   IMM16,  NONE,   SAME,  3},
    {JMP,   REL8,   NONE,   SAME,  2},
    {IN,    REGAL,  REGDX,  SAME,  1},
    {IN,    REGAX,  REGDX,  SAME,  1},
    {OUT,   REGDX,  REGAL,  SAME,  1},
    {OUT,   REGDX,  REGAX,  SAME,  1},

    // 0xF0
    {BADOPCODE},
    {BADOPCODE},
    {BADOPCODE},
    {BADOPCODE},
    {HLT,   NONE,   NONE,   SAME,  1},
    {CMC,   NONE,   NONE,   SAME,  1},
    {BADOPCODE},
    {BADOPCODE},
    {CLC,   NONE,   NONE,   SAME,  1},
    {STC,   NONE,   NONE,   SAME,  1},
    {CLI,   NONE,   NONE,   SAME,  1},
    {STI,   NONE,   NONE,   SAME,  1},
    {CLD,   NONE,   NONE,   SAME,  1},
    {STD,   NONE,   NONE,   SAME,  1},
    {BADOPCODE},
    {BADOPCODE},
};

InsOperand operandParam(InsParamType typ, uint32_t address, uint8_t *imm, InsOperand rm16, InsOperand reg16, InsOperand rm8, InsOperand reg8) {
    InsOperand op;
    op.type = UNUSED;
    switch (typ) {
        case RM8:
            return rm8;
        case RM16:
            return rm16;
        case REG8:
            return reg8;
        case REG16:
            return reg16;
        case IMMU8:
            op.type = IMM;
            op.im = *(uint8_t*)imm;
            break;
        case IMM8:
            op.type = IMM;
            op.im = *(int8_t*)imm;
            break;
        case IMM16:
            op.type = IMM;
            op.im = *(uint16_t*)imm;
            break;
        case IMMU16:
            op.type = IMM;
            op.im = *(int16_t*)imm;
            break;
        case SREG:
            // TODO: handling SREG arguments
            break;
        case REGAX:
        case REGBX:
        case REGCX:
        case REGDX:
        case REGAL:
        case REGBL:
        case REGCL:
        case REGDL:
        case REGAH:
        case REGBH:
        case REGCH:
        case REGDH:
        case REGSI:
        case REGDI:
        case REGBP:
        case REGSP:
            op.type = REG;
            op.r = typ - REGAX;
            break;
        case REL8:
            op.type = IMM;
            op.im = *(int8_t*)imm + address;
            break;
        case REL16:
            op.type = IMM;
            op.im = *(int16_t*)imm + address;
            break;
        case ADDR16:
            op.type = ADDRESS;
            op.r = na;
            op.r2 = na;
            op.im = *(int16_t*)imm;
        default:
            break;
    }
    return op;

}

const char* OPCODENAMES[] = {
    "AAA",   "AAD",   "AAM",   "AAS",   "ADC",   "ADD",   "AND",
    "CALL",  "CBW",   "CLC",   "CLD",   "CLI",   "CMC",   "CMP",
    "CMPSB", "CMPSW", "CWD",   "DAA",   "DAS",   "DEC",   "DIV",
    "HLT",   "IDIV",  "IMUL",  "IN",    "INC",   "INT",   "INTO", "INT3",
    "IRET",  "JA",    "JAE",   "JB",    "JBE",   "JC",    "JCXZ",
    "JE",    "JG",    "JGE",   "JL",    "JLE",   "JMP",   "JNA",
    "JNAE",  "JNB",   "JNBE",  "JNC",   "JNE",   "JNG",   "JNGE",
    "JNL",   "JNLE",  "JNO",   "JNP",   "JNS",   "JNZ",   "JO",
    "JP",    "JPE",   "JPO",   "JS",    "JZ",    "LAHF",  "LDS",
    "LEA",   "LES",   "LODSB", "LODSW", "LOOP",  "LOOPE", "LOOPNE",
    "LOOPNZ","LOOPZ", "MOV",   "MOVSB", "MOVSW", "MUL",   "NEG",
    "NOP",   "NOT",   "OR",    "OUT",   "POP",   "POPA",  "POPF",
    "PUSH",  "PUSHA", "PUSHF", "RCL",   "RCR",   "RET",   "RETF",
    "ROL",   "ROR",   "SAHF",  "SAL",   "SAR",   "SBB",   "SCASB",
    "SCASW", "SHL",   "SHR",   "STC",   "STD",   "STI",   "STOSB",
    "STOSW", "SUB",   "TEST",  "XCHG",  "XLATB", "XOR",
};

void decodedToStr(char *buf, InsDecode *dec) {
    if (dec->type == BADOPCODE) {
        sprintf(buf, "(bad)");
        return;
    }
    char src[64];
    char dst[64];
    char op2[64];
    operandToStr(src, &dec->src);
    operandToStr(dst, &dec->dst);
    operandToStr(op2, &dec->op2);
    sprintf(buf, "%s %s, %s, %s", OPCODENAMES[dec->type], dst, op2, src);
}

InsDecode disassemble(uint8_t **code, uint32_t *address) {
    InsDecode ins;
    uint8_t opcode = *code[0];
    OpcodeTableEntry info = OPCODE_TABLE[opcode];
    if (info.op == BADOPCODE) {
        ins.type = BADOPCODE;
        *code += 1;
        *address += 1;
        return ins;
    }
    InsOperand rm16, reg16, rm8, reg8;
    modrmSib(*code + 1, b8, &reg8, &rm8);
    modrmSib(*code + 1, b16, &reg16, &rm16);
    uint8_t *imm = *code + 1;
    if (info.param1 == REG8 || info.param1 == RM8 || info.param1 == REG16 || info.param1 == RM16) {
        imm++;
    }
    ins.dst = operandParam(info.param1, *address, imm, rm16, reg16, rm8, reg8);
    ins.src = operandParam(info.param2, *address, imm, rm16, reg16, rm8, reg8);
    if (info.param3 == SAME) {
        ins.op2 = ins.dst;
    } else {
        ins.op2 = operandParam(info.param3, *address, imm, rm16, reg16, rm8, reg8);
    }
    ins.type = info.op;
    *code += info.size;
    *address += info.size;
    return ins;
}
