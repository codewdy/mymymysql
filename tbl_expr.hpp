#ifndef WDY_1823162721_TBLEXPR
#define WDY_1823162721_TBLEXPR
#include "table.hpp"
#include <string>

namespace TypeDB {
    struct TblExpr {
        virtual std::vector<pObject> Calc(const Table& tbl);
        virtual ~TblExpr();
    };
    struct ReadTblExpr : public TblExpr {
        std::string tbl;
        std::string desc;
        ReadTblExpr(const std::string& _tbl, const std::string& _desc) : tbl(_tbl), desc(_desc) {}
        ReadTblExpr(const std::string& _desc) : ReadTblExpr("", _desc) {}
        virtual std::vector<pObject> Calc(const Table& tbl);
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
        virtual std::vector<pObject> Calc(const Table& tbl);
        virtual ~UnaryTblExpr();
    };
}

#endif
