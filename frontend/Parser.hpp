#ifndef WDY_2923819213_FRONTEND_PARSER
#define WDY_2923819213_FRONTEND_PARSER
#include "Stmt/stmt.hpp"
#include <string>
namespace Parser {
    Stmt::Stmt* CreateAST(std::string st);
}
#endif
