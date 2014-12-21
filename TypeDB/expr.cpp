#include "expr.hpp"
#include "Exception/Exception.hpp"

namespace TypeDB {
    pObject Expr::Calc(const TableDesc& desc, const Row& row) {
        RAISE(Syntax);
    }

    bool Expr::CalcBool(const TableDesc& desc, const Row& row) {
        RAISE(Syntax);
    }

    Expr::~Expr() {}

    pObject LiteralExpr::Calc(const TableDesc& desc, const Row& row) {
        return Literal;
    }

    pObject ReadExpr::Calc(const TableDesc& desc, const Row& row) {
        return desc.getObject(row, tbl, name);
    }

    bool UnaryExpr::CalcBool(const TableDesc& desc, const Row& row) {
        switch (op) {
            case Not:
                return !l->CalcBool(desc, row);
                break;
        }
    }

    UnaryExpr::~UnaryExpr() {delete l;}

#define CALC2BOOL(op) \
        {\
            bool lhs = l->CalcBool(desc, row), rhs = r->CalcBool(desc, row);\
            return lhs op rhs;\
        }
#define CALC2OBJ(op) \
        {\
            pObject lhs = l->Calc(desc, row), rhs = r->Calc(desc, row);\
            return lhs->op_##op(rhs);\
        }
    pObject BinaryExpr::Calc(const TableDesc& desc, const Row& row) {
        switch (op) {
            case Plus:
                CALC2OBJ(add);
            case Minus:
                CALC2OBJ(minus);
                break;
            default:
                RAISE(Syntax);
        }
    }

    bool BinaryExpr::CalcBool(const TableDesc& desc, const Row& row) {
        switch (op) {
            case Equal:
                CALC2OBJ(eq);
            case NotEqual:
                CALC2OBJ(ne);
            case LessThan:
                CALC2OBJ(lt);
            case GreaterThan:
                CALC2OBJ(gt);
            case LessEqual:
                CALC2OBJ(le);
            case GreaterEqual:
                CALC2OBJ(ge);
            case And:
                CALC2BOOL(&&);
            case Or:
                CALC2BOOL(||);
            default:
                RAISE(Syntax);
        }
    }

    BinaryExpr::~BinaryExpr() {delete l; delete r;}
}
