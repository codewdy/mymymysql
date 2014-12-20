#include "frontend/Parser.hpp"

int main() {
    Parser::CreateAST("SELECT A FROM B WHERE B.id = 3");
}
