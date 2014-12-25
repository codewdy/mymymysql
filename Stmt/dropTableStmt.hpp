#ifndef WDY_1287312763_DROP_TABLE_STMT
#define WDY_1287312763_DROP_TABLE_STMT
#include "stmt.hpp"
namespace Stmt {
    struct DropTableStmt : public Stmt {
        std::string tbl;
        virtual void Run(Context::Context& ctx);
    };
}
#endif
