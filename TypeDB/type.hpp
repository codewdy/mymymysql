#ifndef WDY_9812310210_TYPE
#define WDY_9812310210_TYPE
#include <string>
#include "object.hpp"

namespace TypeDB {
    struct Type {
        virtual void Jump(const char*& buf) = 0;
        virtual Object* CreateAndJump(const char*& buf) = 0;
        virtual bool Test(Object* obj);
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
        virtual void Jump(const char*& buf);
        virtual Object* CreateAndJump(const char*& buf);
        virtual bool Test(Object* obj);
    };
    struct StringType : public Type {
        static String* Create(const std::string& x) {
            return new String(x);
        }
        virtual void Jump(const char*& buf);
        virtual Object* CreateAndJump(const char*& buf);
        virtual bool Test(Object* obj);
    };
    extern IntType* intType;
    extern StringType* stringType;
    void initTypes();
}

#endif
