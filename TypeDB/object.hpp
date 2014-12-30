#ifndef WDY_1293829183_OBJECT
#define WDY_1293829183_OBJECT
#include <string>
#include <memory>
#include "BTree/Hash.hpp"

namespace TypeDB {
    struct Object {
        virtual ~Object();
        virtual bool op_eq(Object* rhs);
        virtual bool op_ne(Object* rhs);
        virtual bool op_lt(Object* rhs);
        virtual bool op_gt(Object* rhs);
        virtual bool op_le(Object* rhs);
        virtual bool op_ge(Object* rhs);
        virtual bool op_like(Object* rhs);
        virtual bool op_is_null(Object* rhs);
        virtual Object* op_add(Object* rhs);
        virtual Object* op_minus(Object* rhs);
        virtual BTree::Key hash();
        virtual void write(char*& buf);
        virtual std::string toString();
    };
    struct Null : public Object {
        virtual bool op_eq(Object* rhs);
        virtual bool op_ne(Object* rhs);
        virtual bool op_lt(Object* rhs);
        virtual bool op_gt(Object* rhs);
        virtual bool op_le(Object* rhs);
        virtual bool op_ge(Object* rhs);
        virtual bool op_like(Object* rhs);
        virtual void write(char*& buf);
        virtual bool op_is_null(Object* rhs);
        virtual BTree::Key hash();
        virtual std::string toString();
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
        virtual BTree::Key hash();
        virtual void write(char*& buf);
        virtual std::string toString();
    };
    struct Double : public Object {
        double raw;
        Double(double _raw) : raw(_raw) {}
        virtual std::string toString();
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
        virtual bool op_like(Object* rhs);
        virtual Object* op_add(Object* rhs);
        virtual BTree::Key hash();
        virtual void write(char*& buf);
        virtual std::string toString();
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
        pObject& operator=(const pObject& rhs) {
            Reduce();
            obj = rhs.obj;
            ref = rhs.ref;
            Increase();
            return *this;
        }
        pObject& operator=(pObject&& rhs) {
            Reduce();
            obj = rhs.obj;
            ref = rhs.ref;
            rhs.obj = nullptr;
            rhs.ref = nullptr;
            return *this;
        }
        Object* operator->() const {return obj;}
        operator Object*() const {return obj;}
    };
}

#endif
