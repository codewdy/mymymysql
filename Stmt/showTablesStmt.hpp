#ifndef WDY_1231782392_SHOW_TABLES
#define WDY_1231782392_SHOW_TABLES
#include "stmt.hpp"
namespace Stmt {
    struct ShowTablesStmt : public Stmt {
        virtual void Run(Context::Context& ctx);
    };
}
#endif
