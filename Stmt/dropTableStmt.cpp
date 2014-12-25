#include "dropTableStmt.hpp"
#include "filter.hpp"

namespace Stmt {
    void DropTableStmt::Run(Context::Context& ctx) {
        ctx.DropTable(tbl);
    }
}
