#ifndef WDY_1293829183_OBJECT
#define WDY_1293829183_OBJECT
#include <string>
#include <memory>

namespace TypeDB {
    struct Object {
        virtual ~Object();
        virtual bool op_eq(Object* rhs);
        virtual bool op_ne(Object* rhs);
        virtual bool op_lt(Object* rhs);
        virtual bool op_gt(Object* rhs);
        virtual bool op_le(Object* rhs);
        virtual bool op_ge(Object* rhs);
        virtual Object* op_add(Object* rhs);
        virtual Object* op_minus(Object* rhs);
    };
    struct Null : public Object {
        virtual bool op_eq(Object* rhs);
        virtual bool op_ne(Object* rhs);
        virtual bool op_lt(Object* rhs);
        virtual bool op_gt(Object* rhs);
        virtual bool op_le(Object* rhs);
        virtual bool op_ge(Object* rhs);
    };
    struct Int : public Object {
        int raw;
        Int(int _raw) : raw(_raw) {}
        virtual bool op_eq(Object* rhs);
        virtual bool op_ne(Object* rhs);
        virtual bool op_lt(Object* rhs);
        virtual bool op_gt(Object* rhs);
        virtual bool op_le(Object* rhs);
        virtual bool op_ge(Object* rhs);
        virtual Object* op_add(Object* rhs);
        virtual Object* op_minus(Object* rhs);
    };
    struct Double : public Object {
        double raw;
        Double(double _raw) : raw(_raw) {}
    };
    struct String : public Object {
        std::string raw;
        String(const std::string& _raw) : raw(_raw) {}
        String(std::string&& _raw) : raw(std::move(_raw)) {}
        virtual bool op_eq(Object* rhs);
        virtual bool op_ne(Object* rhs);
        virtual bool op_lt(Object* rhs);
        virtual bool op_gt(Object* rhs);
        virtual bool op_le(Object* rhs);
        virtual bool op_ge(Object* rhs);
        virtual Object* op_add(Object* rhs);
    };
    struct pObject {
        Object* obj;
        int* ref;
        void Reduce() {if (obj) {(*ref)--;}}
        void Increase() {if (obj) {(*ref)++;}}
        pObject() : obj(nullptr), ref(nullptr) {}
        pObject(Object* _obj) : obj(_obj), ref(new int(1)) {}
        pObject(const pObject& pobj) : obj(pobj.obj), ref(pobj.ref) {Increase();}
        pObject(pObject&& pobj) : obj(pobj.obj), ref(pobj.ref) {pobj.obj = nullptr; pobj.ref = nullptr;}
        Object* operator->() {return obj;}
        operator Object*() {return obj;}
    };
}

#endif
