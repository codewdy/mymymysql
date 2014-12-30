#include "Object.hpp"
#include "Exception/Exception.hpp"
#include "Utils/bufOp.hpp"
#include <vector>

namespace TypeDB {

    Object::~Object() {}
    bool Object::op_eq(Object* rhs) {RAISE(Syntax);}
    bool Object::op_ne(Object* rhs) {RAISE(Syntax);}
    bool Object::op_lt(Object* rhs) {RAISE(Syntax);}
    bool Object::op_gt(Object* rhs) {RAISE(Syntax);}
    bool Object::op_le(Object* rhs) {RAISE(Syntax);}
    bool Object::op_ge(Object* rhs) {RAISE(Syntax);}
    bool Object::op_like(Object* rhs) {RAISE(Syntax);}
    bool Object::op_is_null(Object* rhs) {return false;}
    Object* Object::op_add(Object* rhs) {RAISE(Syntax);}
    Object* Object::op_minus(Object* rhs) {RAISE(Syntax);}

#define DEF_NULL_BOOL_OP(op) \
    bool Null::op_##op(Object* rhs) {return false;}

    DEF_NULL_BOOL_OP(eq);
    DEF_NULL_BOOL_OP(ne);
    DEF_NULL_BOOL_OP(lt);
    DEF_NULL_BOOL_OP(gt);
    DEF_NULL_BOOL_OP(le);
    DEF_NULL_BOOL_OP(ge);
    DEF_NULL_BOOL_OP(like);


#define DEF_BOOL_OP(type, op, opc) \
    bool type::op_##op(Object* rhs) {\
        type* _rhs = dynamic_cast<type*>(rhs);\
        if (_rhs == nullptr) {\
            if (dynamic_cast<Null*>(rhs) == nullptr)\
                RAISE(Syntax);\
            else\
                return false;\
        }\
        return raw opc _rhs->raw;\
    }

#define DEF_OBJ_OP(type, op, opc) \
    Object* type::op_##op(Object* rhs) {\
        type* _rhs = dynamic_cast<type*>(rhs);\
        if (_rhs == nullptr) {\
            RAISE(Syntax);\
        }\
        return new type(raw opc _rhs->raw);\
    }

    DEF_BOOL_OP(Int, eq, ==);
    DEF_BOOL_OP(Int, ne, !=);
    DEF_BOOL_OP(Int, lt, <);
    DEF_BOOL_OP(Int, gt, >);
    DEF_BOOL_OP(Int, le, <=);
    DEF_BOOL_OP(Int, ge, >=);
    DEF_OBJ_OP(Int, add, +);
    DEF_OBJ_OP(Int, minus, -);

    DEF_BOOL_OP(String, eq, ==);
    DEF_BOOL_OP(String, ne, !=);
    DEF_BOOL_OP(String, lt, <);
    DEF_BOOL_OP(String, gt, >);
    DEF_BOOL_OP(String, le, <=);
    DEF_BOOL_OP(String, ge, >=);
    DEF_OBJ_OP(String, add, +);

    bool Null::op_is_null(Object* rhs) {return true;}

    bool String::op_like(Object* rhs) {
        String* _rhs = dynamic_cast<String*>(rhs);
        if (_rhs == nullptr) {
            throw "Type Check Error";
        }
        std::string& r = _rhs->raw;
        std::vector<int> loc;
        loc.push_back(0);
        for (char x : r) {
            if (loc.empty())
                break;
            std::vector<int> locX;
            if (x == '%') {
                for (int i = loc[0]; i <= raw.size(); i++)
                    locX.push_back(i);
            } else {
                for (auto i : loc)
                    if (i < raw.size() && raw[i] == x)
                        locX.push_back(i + 1);
            }
            loc = std::move(locX);
        }
        return !loc.empty() && loc.back() == raw.size();
    }

    BTree::Key Object::hash() {
        RAISE(Syntax);
    }
    BTree::Key Null::hash() {
        return BTree::Key(0x19940319, 0x65766f6c, 0x19941102);
    }
    BTree::Key Int::hash() {
        return BTree::Key(raw);
    }
    BTree::Key String::hash() {
        return BTree::Key(raw);
    }
    void Object::write(char*& buf) {
        RAISE(Syntax);
    }
    void Null::write(char*& buf) {
        Utils::writeInt(buf, 0x80000000);
    }
    void Int::write(char*& buf) {
        Utils::writeInt(buf, raw);
    }
    void String::write(char*& buf) {
        Utils::writeString(buf, raw);
    }
    std::string Object::toString() {
        RAISE(Syntax);
    }
    std::string Null::toString() {
        return "NULL";
    }
    std::string Int::toString() {
        return std::to_string(raw);
    }
    std::string Double::toString() {
        return std::to_string(raw);
    }
    std::string String::toString() {
        return raw;
    }
}
