#ifndef WDY_8718762328_USE_DB_STMT
#define WDY_8718762328_USE_DB_STMT
#include "stmt.hpp"
namespace Stmt {
    struct UseDBStmt : public Stmt {
        std::string db;
        virtual void Run(Context::Context& ctx);
    };
}
#endif
