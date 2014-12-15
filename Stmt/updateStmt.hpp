#ifndef WDY_7321273921_UPDATESTMT
#define WDY_7321273921_UPDATESTMT
#include "stmt.hpp"
#include <unordered_map>
#include "TypeDB/expr.hpp"
namespace Stmt {
    struct UpdateStmt : public Stmt {
        std::string tbl;
        std::unordered_map<std::string, TypeDB::Expr*> rules;
    };
}
#endif
