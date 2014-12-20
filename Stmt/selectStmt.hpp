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
        std::vector<TypeDB::TblExpr*> select;
        std::vector<TypeDB::TableSelector*> from;
        TypeDB::Expr* where;
    };
}
#endif
