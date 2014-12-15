#include "Exception.hpp"

namespace Exception {
    std::string SyntaxException::msg() {
        return "Syntax Error!";
    }
    std::string ColumnNotFoundException::msg() {
        return "Column " + column + " Not Found!";
    }
    std::string ColumnMultiFoundException::msg() {
        return "Column " + column + " Multi Found!";
    }
}
