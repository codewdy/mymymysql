#include "showTablesStmt.hpp"

namespace Stmt {
    void ShowTablesStmt::Run(Context::Context& ctx) {
        auto tbls = ctx.ReadDB();
        for (auto& tbl : tbls)
            std::cout << tbl << std::endl;
    }
}
