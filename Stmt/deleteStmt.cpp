#include "deleteStmt.hpp"
#include "filter.hpp"

namespace Stmt {
    void DeleteStmt::Run(Context::Context& ctx) {
        ctx.AssertTable(tbl);
        TypeDB::Table table = filter(ctx, tbl, where);
        ctx.Delete(tbl, table);
    }
}
