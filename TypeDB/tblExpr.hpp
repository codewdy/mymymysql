#ifndef WDY_1823162721_TBLEXPR
#define WDY_1823162721_TBLEXPR
#include "table.hpp"
#include <string>

namespace TypeDB {
    struct TblExpr {
        virtual std::vector<pObject> Calc(const Table& table) = 0;
        virtual ~TblExpr();
    };
    struct ReadTblExpr : public TblExpr {
        std::string tbl;
        std::string name;
        ReadTblExpr(const std::string& _tbl, const std::string& _name) : tbl(_tbl), name(_name) {}
        ReadTblExpr(const std::string& _name) : tbl(""), name(_name) {}
        virtual std::vector<pObject> Calc(const Table& table);
    };
    struct UnaryTblExpr : public TblExpr {
        TblExpr* l;
        enum OpCode {
            Sum,
            Avg,
            Min,
            Max,
        } op;
        UnaryTblExpr(TblExpr* _l, OpCode _op) : l(_l), op(_op) {}
        virtual std::vector<pObject> Calc(const Table& table);
        virtual ~UnaryTblExpr();
    };
}

#endif
