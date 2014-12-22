#include "updateStmt.hpp"
#include "filter.hpp"

namespace Stmt {
    void UpdateStmt::Run(Context::Context& ctx) {
        TypeDB::Table table = filter(ctx, tbl, where);
        for (auto& rule : rules) {
            int idx = table.desc.getIndex("", rule.first);
            if (idx == table.desc.primaryIndex) {
                //TODO
                throw "Not Imp";
            }
            for (auto& row : table.rows) {
                auto rst = rule.second->Calc(table.desc, row);
                if (!table.desc.descs[idx].type->Test(rst.second.obj)) {
                    //TODO
                    throw "Not Imp";
                }
                row.objs[idx] = rst.second;
            }
        }
        ctx.Update(tbl, table);
    }
}
