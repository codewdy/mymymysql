#ifndef WDY_1829367129_DESC_STMT
#define WDY_1829367129_DESC_STMT
#include "stmt.hpp"
namespace Stmt {
    struct descStmt : public Stmt {
        std::string tbl;
        virtual void Run(Context::Context& ctx);
    };
}
#endif
