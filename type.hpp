#ifndef WDY_9812310210_TYPE
#define WDY_9812310210_TYPE
#include <string>
#include "object.hpp"

namespace TypeDB {
    struct Type {
        virtual char* Jump(char*) = 0;
        virtual std::pair<Object*, char*> CreateAndJump(char*) = 0;
    };
    struct NullType {
        static Null* none;
        static Null* Create() {
            if (!none) {
                none = new Null;
            }
            return none;
        }
    };
    struct IntType : public Type {
        static Int* Create(int x) {
            return new Int(x);
        }
        virtual char* Jump(char*);
        virtual std::pair<Object*, char*> CreateAndJump(char*);
    };
    struct VarCharType : public Type {
        static VarChar* Create(const std::string& x) {
            return new VarChar(x);
        }
        virtual char* Jump(char*);
        virtual std::pair<Object*, char*> CreateAndJump(char*);
    };
}

#endif
