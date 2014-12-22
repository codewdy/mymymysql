#ifndef WDY_9281293742_INSERTSTMT
#define WDY_9281293742_INSERTSTMT
#include "stmt.hpp"
#include "TypeDB/tblExpr.hpp"
#include "TypeDB/tblSelector.hpp"
#include "TypeDB/expr.hpp"
#include <vector>
namespace Stmt {
    struct InsertStmt : public Stmt {
        virtual void Run(Context::Context& ctx);
        std::string tbl_name;
        TypeDB::Table tbl;
    };
}
#endif
