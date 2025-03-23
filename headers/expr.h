#pragma once
#include <stdint.h>
#include "object.h"
typedef enum AtomType { CONST, GLOBAL, LOCAL, TEMP, RETURN } AtomType;
typedef struct ExAtom {
    AtomType typ;
    int32_t val;
    // CONST: val is the constant value
    // GLOBAL: val is the address
    // LOCAL: val is the address, relative to SP
    // TEMP: val is the temp index
    // RETURN: not yet implemented
} ExAtom;
typedef struct Expr Expr;
typedef enum BinType { bADD, bSUB, bMUL, bDIV, bAND, bOR, bXOR, bSHL, bSHR, bANDL, bORL, bEQ, bNEQ, bGT, bLT, bGE, bLE} BinType;
typedef struct ExBin {
    Expr *left;
    Expr *right;
    BinType typ;
} ExBin;
typedef enum UnType { uDEREF, uNEG, uNOT, uNOTL } UnType;
typedef struct ExUn {
    Expr *op;
    UnType typ;
} ExUn;
typedef enum ExprClass { ATOM, BIN, UN } ExprClass;
typedef struct Expr {
    Object obj;
    ExprClass typ;
    union {
        ExAtom atom;
        ExBin bin;
        ExUn un;
    };
} Expr;
static uint32_t num_temps = 0;
Expr *createAtomConst(int32_t val);
Expr *createAtomGlobal(int32_t val);
Expr *createAtomLocal(int32_t val);
Expr *createAtomConst(int32_t val);
Expr *createExprUn(Expr *op, UnType typ);
Expr *createExprBin(Expr *left, BinType typ, Expr *right);
void destroyExpr(Object *old);
