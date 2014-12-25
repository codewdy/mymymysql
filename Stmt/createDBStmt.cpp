#include "createDBStmt.hpp"
#include "filter.hpp"

namespace Stmt {
    void CreateDBStmt::Run(Context::Context& ctx) {
        ctx.CreateDB(db);
    }
}
