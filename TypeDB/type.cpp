#include "type.hpp"
#include "utils/bufOp.hpp"

namespace TypeDB {
    void IntType::Jump(const char*& buf) {
        Utils::jumpInt(buf);
    }
    Object* IntType::CreateAndJump(const char*& buf) {
        int ret = Utils::readInt(buf);
        if (ret == 0x80000000)
            return NullType::none;
        return Create(ret);
    }
    void StringType::Jump(const char*& buf) {
        Utils::jumpString(buf);
    }
    Object* StringType::CreateAndJump(const char*& buf) {
        int size = Utils::readInt(buf);
        if (size == 0x80000000)
            return NullType::none;
        std::string ret(buf, buf + size);
        buf += size;
        return Create(ret);
    }
    bool Type::Test(Object* obj) {
        //TODO
        throw "Not Imp";
    }
    bool StringType::Test(Object* obj) {
        if (obj == NullType::none)
            return null_;
        auto r = dynamic_cast<String*>(obj);
        return r;
    }
    bool IntType::Test(Object* obj) {
        if (obj == NullType::none)
            return null_;
        auto r = dynamic_cast<Int*>(obj);
        return r;
    }
    static Null null;
    Null* NullType::none = &null;
    Type* createIntType(int _desc, bool _null_) {return new IntType(_desc, _null_);}
    Type* createStringType(int _desc, bool _null_) {return new StringType(_desc, _null_);}
    TypeCreator typeCreators[ErrorTypeEnum] = {createIntType, createStringType};
}
