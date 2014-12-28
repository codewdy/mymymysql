#include "descStmt.hpp"

namespace Stmt {
    void descStmt::Run(Context::Context& ctx) {
        auto tbls = ctx.GetTableDesc(tbl);
        for (auto desc : tbls.descs) {
            std::cout << desc.name << " " << desc.type->toString() << std::endl;
        }
    }
}
