#ifndef WDY_8976509898_EXCEPTION
#define WDY_8976509898_EXCEPTION

#include <string>
#include <iostream>

namespace Exception {
    struct Exception {
        virtual std::string msg() = 0;
    };
    struct SyntaxException {
        SyntaxException() {}
        virtual std::string msg();
    };
    struct ColumnNotFoundException {
        std::string column;
        ColumnNotFoundException(const std::string& _column) : column(_column) {}
        virtual std::string msg();
    };
    struct ColumnMultiFoundException {
        std::string column;
        ColumnMultiFoundException(const std::string& _column) : column(_column) {}
        virtual std::string msg();
    };
}


#ifdef DEBUG
#define RAISE(TYPE, ...) do{\
        Exception::TYPE##Exception* ex = new Exception::TYPE##Exception(__VA_ARGS__);\
        std::cerr << "Exception[" << #TYPE << "Exception] Raise At " << __FILE__ << ":" << __LINE__ << std::endl;\
        throw ex;\
}while(0)
#else
#define RAISE(TYPE, ...) do{\
        Exception::TYPE##Exception* ex = new Exception::TYPE##Exception(__VA_ARGS__);\
        throw ex;\
}while(0)
#endif

#endif
