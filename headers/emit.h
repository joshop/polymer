#pragma once
#include "expr.h"
/* Information about emitted decompilation from an instruction:
 *  dest is where this line stores data,
 *  src is what this line stores into dest.
 */
typedef struct Emit {
    Expr *dest;
    Expr *src;
} Emit;
// Destroys an emit.
void destroyEmit(Emit *emit);
// Represents an expression as a string, returning the end of the expression.
char *stringifyExpr(char *buf, Expr *expr);
char *stringifyEmit(char *buf, Emit *emit);
