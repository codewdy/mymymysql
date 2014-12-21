#include "filter.hpp"

namespace Stmt {
    TypeDB::Table filter(const Context::Context& ctx, const std::vector<TypeDB::TableSelector*>& tbls, TypeDB::Expr* cond) {
        //TODO
        return filter(ctx, tbls[0], cond);
    }
    TypeDB::Table filter(const Context::Context& ctx, TypeDB::TableSelector* tbl, TypeDB::Expr* cond) {
        TypeDB::Table ret = tbl->getTable(ctx);
        if (cond) {
            std::vector<TypeDB::Row> rows;
            for (auto& row : ret.rows)
                if (cond->CalcBool(ret.desc, row))
                    rows.push_back(std::move(row));
            ret.rows.swap(rows);
        }
        return ret;
    }
    TypeDB::Table filter(const Context::Context& ctx, const std::string& tblName, TypeDB::Expr* cond) {
        TypeDB::Table ret = ctx.GetTable(tblName);
        if (cond) {
            std::vector<TypeDB::Row> rows;
            for (auto& row : ret.rows)
                if (cond->CalcBool(ret.desc, row))
                    rows.push_back(std::move(row));
            ret.rows.swap(rows);
        }
        return ret;
    }
}
