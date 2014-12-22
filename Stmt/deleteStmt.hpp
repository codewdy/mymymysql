#ifndef WDY_8916271829_DELETESTMT
#define WDY_8916271829_DELETESTMT
#include "stmt.hpp"
#include <unordered_map>
#include "TypeDB/expr.hpp"
namespace Stmt {
    struct DeleteStmt : public Stmt {
        std::string tbl;
        TypeDB::Expr* where;
        virtual void Run(Context::Context& ctx);
    };
}
#endif
