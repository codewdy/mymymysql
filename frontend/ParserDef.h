#ifndef FRONTEND_PARSERDEF
#define FRONTEND_PARSERDEF
#include <string>
struct Lex {
    int type;
    std::string* raw;
};
#endif

