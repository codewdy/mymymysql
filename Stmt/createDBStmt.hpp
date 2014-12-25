#ifndef WDY_1923791823_CREATE_DB_STMT
#define WDY_1923791823_CREATE_DB_STMT
#include "stmt.hpp"
namespace Stmt {
    struct CreateDBStmt : public Stmt {
        std::string db;
        virtual void Run(Context::Context& ctx);
    };
}
#endif
