#include "type.hpp"
#include "utils/bufOp.hpp"

namespace TypeDB {
    void IntType::Jump(const char*& buf) {
        Utils::jumpInt(buf);
    }
    Object* IntType::CreateAndJump(const char*& buf) {
        return new Int(Utils::readInt(buf));
    }
    void StringType::Jump(const char*& buf) {
        Utils::jumpString(buf);
    }
    Object* StringType::CreateAndJump(const char*& buf) {
        return new String(Utils::readString(buf));
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
    static IntType intTypeX;
    static StringType stringTypeX;
    static Null null;
    Null* NullType::none = &null;
    IntType* intType = &intTypeX;
    StringType* stringType = &stringTypeX;
}
