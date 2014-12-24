#include "type.hpp"
#include "utils/bufOp.hpp"

namespace TypeDB {
    void IntType::Jump(const char*& buf) {
        Utils::jumpInt(buf);
    }
    Object* IntType::CreateAndJump(const char*& buf) {
        std::size_t ret = Utils::readInt(buf);
        if (ret == 0x80000000)
            return NullType::none;
        return Create(ret);
    }
    void StringType::Jump(const char*& buf) {
        Utils::jumpString(buf);
    }
    Object* StringType::CreateAndJump(const char*& buf) {
        std::size_t size = Utils::readInt(buf);
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
        auto r = dynamic_cast<String*>(obj);
        return r;
    }
    bool IntType::Test(Object* obj) {
        auto r = dynamic_cast<Int*>(obj);
        return r;
    }
    static Null null;
    Null* NullType::none = &null;
    Type* createIntType(int _desc) {return new IntType(_desc);}
    Type* createStringType(int _desc) {return new StringType(_desc);}
    TypeCreator typeCreators[ErrorTypeEnum] = {createIntType, createStringType};
}
