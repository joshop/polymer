#pragma once
#include <stdint.h>
#include "object.h"
/* The type of an expression atom:
 *  CONST is a constant value,
 *  GLOBAL is a variable stored at a constant address,
 *  LOCAL is a variable stored relative to sp or bp,
 *  TEMP is an internal temporary.
 */
typedef enum AtomType { CONST, GLOBAL, LOCAL, TEMP, RETURN } AtomType;
/* An expression atom:
 *  typ is the type of atom,
 *  val is the value (address, offset, or constant).
 */
typedef struct ExAtom {
    AtomType typ;
    int32_t val;
} ExAtom;
typedef struct Expr Expr;
// A binary operation within an ExBin.
typedef enum BinType { bADD, bSUB, bMUL, bDIV, bAND, bOR, bXOR, bSHL, bSHR, bANDL, bORL, bEQ, bNEQ, bGT, bLT, bGE, bLE} BinType;
/* A binary operation expression node:
 *  left is the left child,
 *  right is the right child,
 *  typ is the type of operation.
 */
typedef struct ExBin {
    Expr *left;
    Expr *right;
    BinType typ;
} ExBin;
// A unary operation within an ExUn.
typedef enum UnType { uDEREF, uNEG, uNOT, uNOTL } UnType;
/* A binary operation expression node:
 *  op is the child,
 *  typ is the type of operation.
 */
typedef struct ExUn {
    Expr *op;
    UnType typ;
} ExUn;
/* The type of an expression node:
 *  ATOM is a leaf node,
 *  BIN is a binary operation,
 *  UN is a unary operation,
 *  UNINIT has not been initialized.
 */
typedef enum ExprClass { ATOM, BIN, UN, UNINIT } ExprClass;
/* An expression node:
 *  obj is the reference counter,
 *  typ is the type of node,
 *  atom, bin, un hold the node itself's data.
 * Expr is reference counted; upon creation it will
 * have no references, and should be tracked with
 * ADDREF and RELEASE.
 */
typedef struct Expr {
    Object obj;
    ExprClass typ;
    union {
        ExAtom atom;
        ExBin bin;
        ExUn un;
    };
} Expr;
// The number of committed internal temporaries.
static uint32_t num_temps = 0;
// Creates a constant expression atom (with no references).
Expr *createAtomConst(int32_t val);
// Creates a global expression atom (with no references).
Expr *createAtomGlobal(int32_t val);
// Creates a local expression atom (with no references).
Expr *createAtomLocal(int32_t val);
// Creates an internal temp expression atom (with no references).
Expr *createAtomTemp(int32_t val);
// Creates a unary expression node (with no references) that operates on op.
Expr *createExprUn(Expr *op, UnType typ);
// Creates a binary expression node (with no references) that operates on left and right.
Expr *createExprBin(Expr *left, BinType typ, Expr *right);
// Creates an uninitialized expression node (with no references).
Expr *createExprUninit();
// Destroys an expression node.
void destroyExpr(Object *old);
// (DEBUG ONLY) Represents an expression as a string.
void exprToStr(char *buf, Expr *expr);
// Creates an expression corresponding to the effective address of expr.
Expr *calcEffAddress(Expr *expr);
