#include <stdint.h>
typedef enum Reg { na, ax, bx, cx, dx, al, bl, cl, dl, ah, bh, ch, dh, si, di, bp, sp, NUM_REGS } Reg;
typedef enum SegReg { ds, es, ss, cs, fs, gs, NUM_SEGREGS } SegReg;
typedef enum OperandType { REG, SEGREG, REGHL, SEGHL, ADDRESS, IMM, REL } OperandType;
typedef enum InsOpcode {
    AAA,   AAD,   AAM,   AAS,   ADC,   ADD,   AND,
    CALL,  CBW,   CLC,   CLD,   CLI,   CMC,   CMP,
    CMPSB, CMPSW, CWD,   DAA,   DAS,   DEC,   DIV,
    HLT,   IDIV,  IMUL,  IN,    INC,   INT,   INTO,
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
    STOSW, SUB,   TEST,  XCHG,  XLATB, XOR
} InsOpcode;
typedef struct InsOperand {
    OperandType type;
    Reg r;
    Reg r2;
    SegReg sr;
    uint32_t im;
} InsOperand;
typedef struct InsDecode {
    InsOperand src;
    InsOperand op2;
    InsOperand dst;
    InsOpcode type;
} InsDecode;
