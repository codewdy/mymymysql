#include "createTableStmt.hpp"
#include "filter.hpp"

namespace Stmt {
    void CreateTableStmt::Run(Context::Context& ctx) {
        ctx.InitTable(tbl, desc);
    }
}
