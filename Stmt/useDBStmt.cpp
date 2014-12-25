#include "useDBStmt.hpp"
#include "filter.hpp"

namespace Stmt {
    void UseDBStmt::Run(Context::Context& ctx) {
        ctx.UseDB(db);
    }
}
