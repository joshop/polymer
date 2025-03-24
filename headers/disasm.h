#pragma once
#include <stdint.h>
typedef uint8_t bool_t;
#define true 1
#define false 0
// An x86 general-purpose register.
typedef enum Reg { ax, bx, cx, dx, al, bl, cl, dl, ah, bh, ch, dh, si, di, bp, sp, NUM_REGS, na } Reg;
// An x86 segment register.
typedef enum SegReg { ds, es, ss, cs, fs, gs, NUM_SEGREGS } SegReg;
/* The type of an x86 operand:
 *  REG is a single register.
 *  SEGREG is a single segment register.
 *  REGHL is a pair of registers (like DX:AX).
 *  SEGHL is a segment:register pair.
 *  ADDRESS is any sort of memory address.
 *  IMM is an immediate value.
 *  REL is a constant relative to the PC.
 */
typedef enum OperandType { UNUSED, REG, SEGREG, REGHL, SEGHL, ADDRESS, IMM, REL } OperandType;
// An x86 opcode.
typedef enum InsOpcode {
    AAA,   AAD,   AAM,   AAS,   ADC,   ADD,   AND,
    CALL,  CBW,   CLC,   CLD,   CLI,   CMC,   CMP,
    CMPSB, CMPSW, CWD,   DAA,   DAS,   DEC,   DIV,
    HLT,   IDIV,  IMUL,  IN,    INC,   INT,   INTO, INT3,
    IRET,  JA,    JAE,   JB,    JBE,   JC,    JCXZ,
    JE,    JG,    JGE,   JL,    JLE,   JMP,   JNA,
    JNAE,  JNB,   JNBE,  JNC,   JNE,   JNG,   JNGE,
    JNL,   JNLE,  JNO,   JNP,   JNS,   JNZ,   JO,
    JP,    JPE,   JPO,   JS,    JZ,    LAHF,  LDS,
    LEA,   LES,   LODSB, LODSW, LOOP,  LOOPE, LOOPNE,
    LOOPNZ,LOOPZ, MOV,   MOVSB, MOVSW, MUL,   NEG,
    NOP,   NOT,   OR,    OUT,   POP,   POPA,  POPF,
    PUSH,  PUSHA, PUSHF, RCL,   RCR,   RET,   RETF,
    ROL,   ROR,   SAHF,  SAL,   SAR,   SBB,   SCASB,
    SCASW, SHL,   SHR,   STC,   STD,   STI,   STOSB,
    STOSW, SUB,   TEST,  XCHG,  XLATB, XOR,
    NUM_OPCODES, BADOPCODE
} InsOpcode;
/* An operand for an x86 instruction:
 *  type indicates the type,
 *  r is the register for REG operands,
 *  r2 is the high register for REGHL operands,
 *  sr is the segment register for SEGREG or SEGHL operands,
 *  im is the immediate value for IMM and REL operands.
 * For operands of type ADDRESS,
 * the effective address is r + r2 + im.
 * r and r2 can be na (equal to zero).
 */
typedef struct InsOperand {
    OperandType type;
    Reg r;
    Reg r2;
    SegReg sr;
    uint32_t im;
    uint8_t scale;
} InsOperand;
/* A decoded x86 instruction:
 *  src is the source operand,
 *  op2 is the second operand (usually same as dst),
 *  dst is the destination operand,
 *  type is the opcode,
 *  flags describe opcode behavior.
 */
typedef struct InsDecode {
    InsOperand src;
    InsOperand op2;
    InsOperand dst;
    InsOpcode type;
    uint32_t flags;
    uint32_t component;
} InsDecode;
// IFLAGS_JUMP indicates that the instruction jumps to its dst->im.
#define IFLAGS_JUMP 1
// IFLAGS_NONEXT indicates that the instruction never proceeds to the instruction following it (e.g. RET, JMP)
#define IFLAGS_NONEXT 2
// Disassembles and decodes one instruction, updating code and address.
InsDecode disassembleSingle(uint8_t **code, uint32_t *address);
// (DEBUG ONLY) Represents an instruction as a string.
void decodedToStr(char *buf, InsDecode *dec);
