#include "../headers/expr.h"
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
void destroyExpr(Object *old) {
    Expr *ex = (Expr*)old;
    if (ex->typ == UN) {
        RELEASE(ex->un.op);
    } else if (ex->typ == BIN) {
        RELEASE(ex->bin.left);
        RELEASE(ex->bin.right);
    }
}
