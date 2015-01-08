#include "updateStmt.hpp"
#include "filter.hpp"

namespace Stmt {
    void UpdateStmt::Run(Context::Context& ctx) {
        ctx.AssertTable(tbl);
        TypeDB::Table table = filter(ctx, tbl, where);
        for (auto& rule : rules) {
            int idx = table.desc.getIndex("", rule.first);
            if (idx == table.desc.primaryIndex) {
                throw "Cannot Update Primary Key";
            }
            for (auto& row : table.rows) {
                auto rst = rule.second->Calc(table.desc, row);
                if (!table.desc.descs[idx].type->Test(rst.second.obj)) {
                    throw "Type Check Error";
                }
                row.objs[idx] = rst.second;
            }
        }
        ctx.Update(tbl, table);
    }
}
