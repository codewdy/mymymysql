#ifndef WDY_1923187232_CREATETABLESTMT
#define WDY_1923187232_CREATETABLESTMT
#include "stmt.hpp"
#include <unordered_map>
#include "TypeDB/expr.hpp"
namespace Stmt {
    struct CreateTableStmt : public Stmt {
        std::string tbl;
        TypeDB::TableDesc desc;
        virtual void Run(Context::Context& ctx);
    };
}
#endif
