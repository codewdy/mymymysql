#include "SelectStmt.hpp"
#include <iostream>

namespace Stmt {
    void SelectStmt::Run(Context::Context& ctx) {
        auto tbl = from[0]->getTable(ctx);
        if (selectAll) {
            for (auto row : tbl.rows) {
                for (auto item : row.objs)
                    std::cout << item->toString() << " ";
                std::cout << std::endl;
            }
        }
    }
}
