#include "frontend/Parser.hpp"

int main() {
    Parser::CreateAST("select a from b where b.id = 3 and (b.name = \" \" or b.name = \"a\")");
}
