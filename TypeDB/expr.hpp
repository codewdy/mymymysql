#ifndef WDY_0293812012_EXPR
#define WDY_0293812012_EXPR
#include "table.hpp"

namespace TypeDB {
    struct Expr {
        virtual pObject Calc(const Row& row);
        virtual bool CalcBool(const Row& row);
        virtual ~Expr();
    };
    struct LiteralExpr : public Expr {
        pObject Literal;
        LiteralExpr(const pObject& l) : Literal(l) {}
        LiteralExpr(pObject&& l) : Literal(std::move(l)) {}
        virtual pObject Calc(const Row& row);
    };
    struct ReadExpr : public Expr {
        std::string tbl, name;
        ReadExpr(const std::string& _tbl, const std::string& _name) : tbl(_tbl), name(_name) {}
        ReadExpr(const std::string& _name) : tbl(""), name(_name) {}
        virtual pObject Calc(const Row& row);
    };
    struct UnaryExpr : public Expr {
        Expr *l;
        enum OpCode {
            Not,
        } op;
        UnaryExpr(Expr* _l, OpCode _op) : l(_l), op(_op) {}
        virtual bool CalcBool(const Row& row);
        virtual ~UnaryExpr();
    };
    struct BinaryExpr : public Expr {
        Expr *l, *r;
        enum OpCode {
            Plus,
            Minus,
            Equal,
            NotEqual,
            LessThan,
            GreaterThan,
            LessEqual,
            GreaterEqual,
            And,
            Or,
        } op;
        BinaryExpr(Expr* _l, Expr* _r, OpCode _op) : l(_l), r(_r), op(_op) {}
        virtual pObject Calc(const Row& row);
        virtual bool CalcBool(const Row& row);
        virtual ~BinaryExpr();
    };
}

#endif
