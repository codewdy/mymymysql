#include "insertStmt.hpp"

namespace Stmt {
    void InsertStmt::Run(Context::Context& ctx) {
        ctx.Insert(tbl_name, tbl);
    }
}
