#ifndef WDY_9812310210_TYPE
#define WDY_9812310210_TYPE
#include <string>
#include "object.hpp"

namespace TypeDB {
    enum TypeEnum : unsigned char {
        IntEnum,
        StringEnum,
        ErrorTypeEnum,
    };
    struct Type {
        int desc;
        TypeEnum type;
        Type(int _desc = 0, TypeEnum _type = ErrorTypeEnum) : desc(_desc), type(_type) {}
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
        IntType(int _desc) : Type(_desc, IntEnum) {}
        virtual void Jump(const char*& buf);
        virtual Object* CreateAndJump(const char*& buf);
        virtual bool Test(Object* obj);
    };
    struct StringType : public Type {
        static String* Create(const std::string& x) {
            return new String(x);
        }
        StringType(int _desc) : Type(_desc, StringEnum) {}
        virtual void Jump(const char*& buf);
        virtual Object* CreateAndJump(const char*& buf);
        virtual bool Test(Object* obj);
    };
    void initTypes();
    typedef Type* (*TypeCreator)(int _desc);
    extern TypeCreator typeCreators[ErrorTypeEnum];
}

#endif
