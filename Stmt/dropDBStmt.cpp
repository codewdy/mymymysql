#include "dropDBStmt.hpp"
#include "filter.hpp"

namespace Stmt {
    void DropDBStmt::Run(Context::Context& ctx) {
        ctx.DropDB(db);
    }
}
