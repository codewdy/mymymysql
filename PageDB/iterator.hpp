#ifndef WDY_0192102912_PAGEDB_ITERATOR
#define WDY_0192102912_PAGEDB_ITERATOR
#include "file.hpp"
#include "page.hpp"
#include "scheduler.hpp"
namespace PageDB {
    struct Iterator {
        Scheduler* pgdb;
        File* file;
        PageDB::Location loc;
        PageWriteSession session;
        Iterator(Scheduler* _pgdb, File* _file, int _pageid = -1, int _offset = 0)
            : pgdb(_pgdb), file(_file),
            loc(_pageid, _offset), session(pgdb->GetWriteSession(file, _pageid))
        {}
        Iterator(Scheduler* _pgdb, File* _file, Location _loc)
            : pgdb(_pgdb), file(_file),
            loc(_loc), session(pgdb->GetWriteSession(file, _loc.Page))
        {}
        void Goto(unsigned short page, unsigned short offset) {
            Goto(PageDB::Location(page, offset));
        }
        void Goto(Location _loc) {
            if (loc.Page != _loc.Page) {
                session = pgdb->GetWriteSession(file, _loc.Page);
            }
            loc = _loc;
        }
        char* Get() {
            return session.buf() + loc.Offset;
        }
        void Next() {
            auto nxt = NextLocation();
            Goto(nxt);
        }
        virtual Location NextLocation() {
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
        Location loc;
        PageSession session;
        ConstIterator(Scheduler* _pgdb, File* _file, int _pageid = -1, int _offset = 0)
            : pgdb(_pgdb), file(_file),
            loc(_pageid, _offset), session(pgdb->GetSession(file, _pageid))
        {}
        ConstIterator(Scheduler* _pgdb, File* _file, Location _loc)
            : pgdb(_pgdb), file(_file),
            loc(_loc), session(pgdb->GetSession(file, _loc.Page))
        {}
        void Goto(unsigned short page, unsigned short offset) {
            Goto(PageDB::Location(page, offset));
        }
        void Goto(Location _loc) {
            if (loc.Page != _loc.Page) {
                session = pgdb->GetSession(file, _loc.Page);
            }
            loc = _loc;
        }
        const char* Get() {
            return session.buf() + loc.Offset;
        }
        void Next() {
            auto nxt = NextLocation();
            Goto(nxt);
        }
        virtual Location NextLocation() {
            //TODO: Not Imp
            throw "Not Imp";
        }
        const char& operator[](int idx) {
            return Get()[idx];
        }
    };
    inline bool operator==(const Iterator& lhs, const Iterator& rhs) {
        return lhs.file == rhs.file && lhs.loc == rhs.loc;
    }
    inline bool operator!=(const Iterator& lhs, const Iterator& rhs) {
        return !(lhs == rhs);
    }
    inline bool operator==(const ConstIterator& lhs, const ConstIterator& rhs) {
        return lhs.file == rhs.file && lhs.loc == rhs.loc;
    }
    inline bool operator!=(const ConstIterator& lhs, const ConstIterator& rhs) {
        return !(lhs == rhs);
    }
    inline bool operator==(const Iterator& lhs, const ConstIterator& rhs) {
        return lhs.file == rhs.file && lhs.loc == rhs.loc;
    }
    inline bool operator!=(const Iterator& lhs, const ConstIterator& rhs) {
        return !(lhs == rhs);
    }
    inline bool operator==(const ConstIterator& lhs, const Iterator& rhs) {
        return lhs.file == rhs.file && lhs.loc == rhs.loc;
    }
    inline bool operator!=(const ConstIterator& lhs, const Iterator& rhs) {
        return !(lhs == rhs);
    }
}
#endif
