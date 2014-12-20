#ifndef WDY_0293192102_STMT
#define WDY_0293192102_STMT
#include "context/context.hpp"
namespace Stmt {
    struct Stmt {
        virtual void Run(Context::Context& ctx);
        virtual ~Stmt() {}
    };
}
#endif
