#ifndef WDY_0392129210_SELECTSTMT
#define WDY_0392129210_SELECTSTMT
#include "stmt.hpp"
#include "TypeDB/tblExpr.hpp"
#include "TypeDB/tblSelector.hpp"
#include "TypeDB/expr.hpp"
#include <vector>
namespace Stmt {
    struct SelectStmt : public Stmt {
        virtual void Run(Context::Context& ctx);
        void Output(const TypeDB::Table& tbl);
        std::vector<TypeDB::TblExpr*> select;
        bool selectAll;
        TypeDB::Expr* groupby;
        std::vector<TypeDB::TableSelector*> from;
        TypeDB::Expr* where;
        SelectStmt() : selectAll(false), groupby(nullptr), where(nullptr) {}
    };
}
#endif
