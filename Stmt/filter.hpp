#ifndef WDY_9102392182_STMT_FILTER
#define WDY_9102392182_STMT_FILTER
#include "TypeDB/table.hpp"
#include "TypeDB/tblSelector.hpp"
#include "TypeDB/expr.hpp"
#include "context/context.hpp"
namespace Stmt {
    TypeDB::Table filter(const Context::Context& ctx, const std::vector<TypeDB::TableSelector*>& tbls, TypeDB::Expr* cond = nullptr);
    TypeDB::Table filter(const Context::Context& ctx, TypeDB::TableSelector* tbl, TypeDB::Expr* cond = nullptr);
    TypeDB::Table filter(const Context::Context& ctx, const std::string& tblName, TypeDB::Expr* cond = nullptr);
}
#endif
