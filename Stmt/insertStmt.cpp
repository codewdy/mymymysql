#include "insertStmt.hpp"

namespace Stmt {
    void InsertStmt::Run(Context::Context& ctx) {
        ctx.AssertTable(tbl_name);
        ctx.Insert(tbl_name, tbl);
    }
}
