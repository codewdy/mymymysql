#include "filter.hpp"

namespace Stmt {
    TypeDB::Table filter(const Context::Context& ctx, const std::vector<TypeDB::TableSelector*>& tbls, TypeDB::Expr* cond, bool force) {
        auto ret = filter(ctx, tbls[0], cond, true);
        for (std::size_t i = 1; i < tbls.size(); i++) {
            auto new_tbl = filter(ctx, tbls[i], cond, true);
            auto desc = TypeDB::TableDesc::merge(ret.desc, new_tbl.desc);
            ret.desc = desc;
            std::vector<TypeDB::Row> rows;
            for (auto& row1 : ret.rows)
                for (auto& row2 : new_tbl.rows) {
                    auto row = TypeDB::Row::merge(row1, row2);
                    if (cond) {
                        auto x = cond->CalcBool(desc, row);
                        if (!x.first || x.second)
                            rows.push_back(row);
                    } else {
                        rows.push_back(row);
                    }
                }
            ret.rows.swap(rows);
        }
        if (cond && !force) {
            for (auto& row : ret.rows)
                if (!cond->CalcBool(ret.desc, row).first) {
                    throw "Column Not Found";
                }
        }
        return std::move(ret);
    }
    TypeDB::Table filter(const Context::Context& ctx, TypeDB::TableSelector* tbl, TypeDB::Expr* cond, bool force) {
        TypeDB::Table ret = tbl->getTable(ctx);
        if (cond) {
            std::vector<TypeDB::Row> rows;
            for (auto& row : ret.rows) {
                auto x = cond->CalcBool(ret.desc, row);
                if (!force && !x.first) {
                    throw "Column Not Found";
                }
                if (!x.first || x.second)
                    rows.push_back(std::move(row));
            }
            ret.rows.swap(rows);
        }
        return std::move(ret);
    }
    TypeDB::Table filter(const Context::Context& ctx, const std::string& tblName, TypeDB::Expr* cond, bool force) {
        TypeDB::Table ret = ctx.GetTable(tblName);
        if (cond) {
            std::vector<TypeDB::Row> rows;
            for (auto& row : ret.rows) {
                auto x = cond->CalcBool(ret.desc, row);
                if (force && !x.first) {
                    throw "Column Not Found";
                }
                if (!x.first || x.second)
                    rows.push_back(std::move(row));
            }
            ret.rows.swap(rows);
        }
        return std::move(ret);
    }
}
