#ifndef WDY_0192102912_PAGEDB_ITERATOR
#define WDY_0192102912_PAGEDB_ITERATOR
#include "file.hpp"
#include "page.hpp"
#include "scheduler.hpp"
namespace PageDB {
    struct Iterator {
        Scheduler* pgdb;
        File* file;
        int pageid, offset;
        PageWriteSession session;
        Iterator(Scheduler* _pgdb, const std::string& fn, int _pageid = -1, int _offset = 0)
            : pgdb(_pgdb), file(pgdb->OpenFile(fn)),
            pageid(_pageid), offset(_offset), session(pgdb->GetWriteSession(file, pageid))
        {}
        Iterator(Scheduler* _pgdb, File* _file, int _pageid = -1, int _offset = 0)
            : pgdb(_pgdb), file(_file),
            pageid(_pageid), offset(_offset), session(pgdb->GetWriteSession(file, pageid))
        {}
        void Goto(int _pageid, int _offset) {
            if (pageid != _pageid) {
                pageid = _pageid;
                session = pgdb->GetWriteSession(file, pageid);
            }
            offset = _offset;
        }
        char* Get() {
            return session.buf() + offset;
        }
        void Next() {
            auto nxt = NextLocation();
            Goto(nxt.first, nxt.second);
        }
        bool End() {
            return pageid < 0;
        }
        virtual std::pair<int, int> NextLocation() {
            //TODO: Not Imp
            throw "Not Imp";
        }
        char& operator[](int idx) {
            return Get()[idx];
        }
    };
    struct ConstIterator {
        Scheduler* pgdb;
        File* file;
        int pageid, offset;
        PageSession session;
        ConstIterator(Scheduler* _pgdb, const std::string& fn, int _pageid = -1, int _offset = 0)
            : pgdb(_pgdb), file(pgdb->OpenFile(fn)),
            pageid(_pageid), offset(_offset), session(pgdb->GetSession(file, pageid))
        {}
        ConstIterator(Scheduler* _pgdb, File* _file, int _pageid = -1, int _offset = 0)
            : pgdb(_pgdb), file(_file),
            pageid(_pageid), offset(_offset), session(pgdb->GetSession(file, pageid))
        {}
        void Goto(int _pageid, int _offset) {
            if (pageid != _pageid) {
                pageid = _pageid;
                session = pgdb->GetSession(file, pageid);
            }
            offset = _offset;
        }
        const char* Get() {
            return session.buf() + offset;
        }
        void Next() {
            auto nxt = NextLocation();
            Goto(nxt.first, nxt.second);
        }
        bool End() {
            return pageid < 0;
        }
        virtual std::pair<int, int> NextLocation() {
            //TODO: Not Imp
            throw "Not Imp";
        }
        const char& operator[](int idx) {
            return Get()[idx];
        }
    };
    bool operator==(const Iterator& lhs, const Iterator& rhs) {
        return lhs.file == rhs.file && lhs.pageid == rhs.pageid && lhs.offset == rhs.offset;
    }
    bool operator!=(const Iterator& lhs, const Iterator& rhs) {
        return !(lhs == rhs);
    }
    bool operator==(const ConstIterator& lhs, const ConstIterator& rhs) {
        return lhs.file == rhs.file && lhs.pageid == rhs.pageid && lhs.offset == rhs.offset;
    }
    bool operator!=(const ConstIterator& lhs, const ConstIterator& rhs) {
        return !(lhs == rhs);
    }
}
#endif
