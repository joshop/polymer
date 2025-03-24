#include "../headers/expr.h"
#include <stdio.h>
#include <assert.h>
Expr *createAtomConst(int32_t val) {
    Expr *new = CREATE(Expr);
    new->typ = ATOM;
    new->atom.typ = CONST;
    new->atom.val = val;
    new->obj.destroy = destroyExpr;
    return new;
}
Expr *createAtomLocal(int32_t val) {
    Expr *new = CREATE(Expr);
    new->typ = ATOM;
    new->atom.typ = LOCAL;
    new->atom.val = val;
    new->obj.destroy = destroyExpr;
    return new;
}
Expr *createAtomGlobal(int32_t val) {
    Expr *new = CREATE(Expr);
    new->typ = ATOM;
    new->atom.typ = GLOBAL;
    new->atom.val = val;
    new->obj.destroy = destroyExpr;
    return new;
}
Expr *createAtomTemp(int32_t val) {
    Expr *new = CREATE(Expr);
    new->typ = ATOM;
    new->atom.typ = TEMP;
    new->atom.val = val;
    new->obj.destroy = destroyExpr;
    return new;
}
uint32_t nextTemp() {
    return num_temps++;
}
Expr *createExprUn(Expr *op, UnType typ) {
    Expr *new = CREATE(Expr);
    new->typ = UN;
    new->un.typ = typ;
    new->un.op = op;
    new->obj.destroy = destroyExpr;
    ADDREF(op);
    return new;
}
Expr *createExprBin(Expr *left, BinType typ, Expr *right) {
    Expr *new = CREATE(Expr);
    new->typ = BIN;
    new->bin.typ = typ;
    new->bin.left = left;
    new->bin.right = right;
    new->obj.destroy = destroyExpr;
    ADDREF(left);
    ADDREF(right);
    return new;
}
Expr *createExprUninit() {
    Expr *new = CREATE(Expr);
    new->typ = UNINIT;
    return new;
}
void destroyExpr(Object *old) {
    Expr *ex = (Expr*)old;
    if (ex->typ == UN) {
        RELEASE(ex->un.op);
    } else if (ex->typ == BIN) {
        RELEASE(ex->bin.left);
        RELEASE(ex->bin.right);
    }
}
Expr *calcEffAddress(Expr *expr) {
    if (expr->typ == UN && expr->un.typ == uDEREF) {
        //ADDREF(expr->un.op);
        return expr->un.op;
    } else if (expr->typ == ATOM) {
        if (expr->atom.typ == GLOBAL) {
            return createAtomConst(expr->atom.val);
        } else if (expr->atom.typ == LOCAL) {
            // TODO: make sp more real
            assert(0);
        }
    }
    assert(0);
}
const char *UNARYOPS[] = {"*", "-", "~", "!"};
const char *BINARYOPS[] = {"+", "-", "*", "/", "&", "|", "^", "<<", ">>", "&&", "||", "==", "!=", ">", "<", ">=", "<="};
void exprToStr(char *buf, Expr *expr) {
    char buf2[1024];
    char buf3[1024];
    switch(expr->typ) {
        case UNINIT:
            sprintf(buf, "???");
            break;
        case ATOM:
            if (expr->atom.typ == CONST) {
                sprintf(buf, "0x%x", expr->atom.val);
            } else if (expr->atom.typ == GLOBAL) {
                sprintf(buf, "global_%x", expr->atom.val);
            } else if (expr->atom.typ == LOCAL) {
                sprintf(buf, "local_%x", expr->atom.val);
            } else if (expr->atom.typ == TEMP) {
                sprintf(buf, "temp%d", expr->atom.val);
            }
            break;
        case UN:
            exprToStr(buf2, expr->un.op);
            sprintf(buf, "%s(%s)", UNARYOPS[expr->un.typ], buf2);
            break;
        case BIN:
            exprToStr(buf2, expr->bin.left);
            exprToStr(buf3, expr->bin.right);
            sprintf(buf, "(%s) %s (%s)", buf2, BINARYOPS[expr->bin.typ], buf3);
            break;
        default:
            return;
    }
}
