#include "expr.hpp"
#include "Exception/Exception.hpp"

namespace TypeDB {
    std::pair<bool, pObject> Expr::Calc(const TableDesc& desc, const Row& row) {
        RAISE(Syntax);
    }

    std::pair<bool, bool> Expr::CalcBool(const TableDesc& desc, const Row& row) {
        RAISE(Syntax);
    }

    Expr::~Expr() {}

    std::pair<bool, pObject> LiteralExpr::Calc(const TableDesc& desc, const Row& row) {
        return std::make_pair(true, Literal);
    }

    std::pair<bool, pObject> ReadExpr::Calc(const TableDesc& desc, const Row& row) {
        auto ret = desc.getObject(row, tbl, name, true);
        if (ret.obj == nullptr)
            return std::make_pair(false, ret);
        else
            return std::make_pair(true, ret);
    }

    std::pair<bool, bool> UnaryExpr::CalcBool(const TableDesc& desc, const Row& row) {
        switch (op) {
            case Not:
                {
                    auto X = l->CalcBool(desc, row);
                    X.second = !X.second;
                    return X;
                }
        }
    }

    UnaryExpr::~UnaryExpr() {delete l;}

#define CALC2BOOL(op, def) \
        {\
            auto lhs = l->CalcBool(desc, row), rhs = r->CalcBool(desc, row);\
            if (lhs.first && rhs.first)\
                return std::make_pair(true, lhs.second op rhs.second);\
            else\
                return std::make_pair(false, def);\
        }
#define CALC2OBJ(op, def) \
        {\
            auto lhs = l->Calc(desc, row), rhs = r->Calc(desc, row);\
            if (lhs.first && rhs.first)\
                return std::make_pair(true, lhs.second->op_##op(rhs.second));\
            else\
                return std::make_pair(false, def);\
        }
    std::pair<bool, pObject> BinaryExpr::Calc(const TableDesc& desc, const Row& row) {
        switch (op) {
            case Plus:
                CALC2OBJ(add, nullptr);
            case Minus:
                CALC2OBJ(minus, nullptr);
                break;
            default:
                RAISE(Syntax);
        }
    }

    std::pair<bool, bool> BinaryExpr::CalcBool(const TableDesc& desc, const Row& row) {
        switch (op) {
            case Equal:
                CALC2OBJ(eq, false);
            case NotEqual:
                CALC2OBJ(ne, false);
            case LessThan:
                CALC2OBJ(lt, false);
            case GreaterThan:
                CALC2OBJ(gt, false);
            case LessEqual:
                CALC2OBJ(le, false);
            case GreaterEqual:
                CALC2OBJ(ge, false);
            case And:
                {
                    auto lhs = l->CalcBool(desc, row), rhs = r->CalcBool(desc, row);
                    if (lhs.first && rhs.first)
                        return std::make_pair(true, lhs.second && rhs.second);
                    if (!lhs.first && !rhs.first)
                        return std::make_pair(false, false);
                    if (lhs.first)
                        return std::make_pair(!lhs.second, lhs.second);
                    if (rhs.first)
                        return std::make_pair(!rhs.second, rhs.second);
                }
            case Or:
                {
                    auto lhs = l->CalcBool(desc, row), rhs = r->CalcBool(desc, row);
                    if (lhs.first && rhs.first)
                        return std::make_pair(true, lhs.second || rhs.second);
                    if (!lhs.first && !rhs.first)
                        return std::make_pair(false, false);
                    if (lhs.first)
                        return std::make_pair(lhs.second, lhs.second);
                    if (rhs.first)
                        return std::make_pair(rhs.second, rhs.second);
                }
            default:
                RAISE(Syntax);
        }
    }

    BinaryExpr::~BinaryExpr() {delete l; delete r;}
}
