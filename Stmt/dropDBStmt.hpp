#ifndef WDY_1278936123_DROP_DB_STMT
#define WDY_1278936123_DROP_DB_STMT
#include "stmt.hpp"
namespace Stmt {
    struct DropDBStmt : public Stmt {
        std::string db;
        virtual void Run(Context::Context& ctx);
    };
}
#endif
