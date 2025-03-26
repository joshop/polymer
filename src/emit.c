#include "../headers/emit.h"
#include <stdio.h>
#include <string.h>

void destroyEmit(Emit *emit) {
    if (emit->dest != NULL) {
        RELEASE(emit->dest);
        RELEASE(emit->src);
    }
}
const uint8_t UNARYPREC[] = {2, 2, 2, 2};
const uint8_t BINARYPREC[] = {4, 4, 3, 3, 8, 10, 9, 5, 5, 11, 12, 7, 7, 6, 6, 6, 6};
extern const char *UNARYOPS[];
extern const char *BINARYOPS[];
const uint8_t ATOMPREC = 1;
char *stringifyAtom(char *buf, ExAtom atom) {
    // TODO: symbols
    if (atom.typ == CONST) {
        return buf + sprintf(buf, "0x%x", atom.val);
    } else if (atom.typ == GLOBAL) {
        return buf + sprintf(buf, "global_%x", atom.val);
    } else if (atom.typ == LOCAL) {
        return buf + sprintf(buf, "local_%x", atom.val);
    } else if (atom.typ == TEMP) {
        return buf + sprintf(buf, "temp%d", atom.val);
    } else {
        return buf;
    }
}
uint8_t precedenceLevel(Expr *expr) {
    if (expr->typ == ATOM) {
        return ATOMPREC;
    } else if (expr->typ == UN) {
        return UNARYPREC[expr->un.typ];
    } else if (expr->typ == BIN) {
        return BINARYPREC[expr->bin.typ];
    } else {
        return 1;
    }
}
char *stringifyExpr(char *buf, Expr *expr) {
    // TODO: memory safety
    uint8_t prec = precedenceLevel(expr);
    if (expr->typ == ATOM) {
        return stringifyAtom(buf, expr->atom);
    } else if (expr->typ == UN) {
        uint8_t subPrec = precedenceLevel(expr->un.op);
        size_t oplen = strlen(UNARYOPS[expr->un.typ]);
        memcpy(buf, UNARYOPS[expr->un.typ], oplen);
        buf += oplen;
        if (prec < subPrec) {
            *(buf++) = '(';
        }
        buf = stringifyExpr(buf, expr->un.op);
        if (prec < subPrec) {
            *(buf++) = ')';
        }
        return buf;
    } else if (expr->typ == BIN) {
        uint8_t leftPrec = precedenceLevel(expr->bin.left);
        uint8_t rightPrec = precedenceLevel(expr->bin.right);
        size_t oplen = strlen(BINARYOPS[expr->bin.typ]);
        if (prec < leftPrec) {
            *(buf++) = '(';
        }
        buf = stringifyExpr(buf, expr->bin.left);
        if (prec < leftPrec) {
            *(buf++) = ')';
        }
        memcpy(buf, BINARYOPS[expr->bin.typ], oplen);
        buf += oplen;
        if (prec < rightPrec) {
            *(buf++) = '(';
        }
        buf = stringifyExpr(buf, expr->bin.right);
        if (prec < rightPrec) {
            *(buf++) = ')';
        }
        return buf;
    } else {
        return buf + sprintf(buf, "???");
    }
}
char *stringifyEmit(char *buf, Emit *emit) {
    if (emit->dest == NULL) {
        return buf;
    }
    buf = stringifyExpr(buf, emit->dest);
    buf = buf + sprintf(buf, " = ");
    buf = stringifyExpr(buf, emit->src);
    return buf + sprintf(buf, ";\n");
}
