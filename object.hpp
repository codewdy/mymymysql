#ifndef WDY_1293829183_OBJECT
#define WDY_1293829183_OBJECT
#include <string>
#include <memory>

namespace TypeDB {
    struct Object;
    typedef std::shared_ptr<Object> pObject;
    struct Object {
        virtual bool op_eq(Object*);
        virtual bool op_ne(Object*);
        virtual bool op_lt(Object*);
        virtual bool op_gt(Object*);
        virtual bool op_le(Object*);
        virtual bool op_ge(Object*);
        virtual Object* op_add(Object*);
        virtual Object* op_minus(Object*);
    };
    struct Null : public Object {
        virtual bool op_eq(Object*);
        virtual bool op_ne(Object*);
        virtual bool op_lt(Object*);
        virtual bool op_gt(Object*);
        virtual bool op_le(Object*);
        virtual bool op_ge(Object*);
    };
    struct Int : public Object {
        int raw;
        Int(int _raw) : raw(_raw) {}
        virtual bool op_eq(Object*);
        virtual bool op_ne(Object*);
        virtual bool op_lt(Object*);
        virtual bool op_gt(Object*);
        virtual bool op_le(Object*);
        virtual bool op_ge(Object*);
        virtual Object* op_add(Object*);
        virtual Object* op_minus(Object*);
    };
    struct VarChar : public Object {
        std::string raw;
        VarChar(const std::string& _raw) : raw(_raw) {}
        VarChar(std::string&& _raw) : raw(std::move(_raw)) {}
        virtual bool op_eq(Object*);
        virtual bool op_ne(Object*);
        virtual bool op_lt(Object*);
        virtual bool op_gt(Object*);
        virtual bool op_le(Object*);
        virtual bool op_ge(Object*);
        virtual Object* op_add(Object*);
    };
}

#endif
