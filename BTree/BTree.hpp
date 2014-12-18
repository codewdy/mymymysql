#ifndef WDY_9123193456_BTREE
#define WDY_9123193456_BTREE

#include "PageDB/scheduler.hpp"
#include "PageDB/iterator.hpp"
#include "Base.hpp"
#include <vector>

namespace BTree {
    struct BTreeIterator : public PageDB::Iterator {
        BTreeIterator(PageDB::Scheduler* _pgdb, PageDB::File* _file, int _pageid, int _offset)
            : PageDB::Iterator(_pgdb, _file, _pageid, _offset) {}
        BTreeIterator(PageDB::Scheduler* _pgdb, PageDB::File* _file, PageDB::Location _loc)
            : PageDB::Iterator(_pgdb, _file, _loc) {}
        Information& Info();
        Value& value();
        virtual PageDB::Location NextLocation();
    };
    struct BTreeConstIterator : public PageDB::ConstIterator {
        BTreeConstIterator(PageDB::Scheduler* _pgdb, PageDB::File* _file, int _pageid, int _offset)
            : PageDB::ConstIterator(_pgdb, _file, _pageid, _offset) {}
        BTreeConstIterator(PageDB::Scheduler* _pgdb, PageDB::File* _file, PageDB::Location _loc)
            : PageDB::ConstIterator(_pgdb, _file, _loc) {}
        const Information& Info();
        Value value();
        virtual PageDB::Location NextLocation();
        BTreeIterator toWritable() {
            return BTreeIterator(pgdb, file, loc);
        }
    };
    const int MagicNumber = 0x19940319;
    const int MagicNumberOffset = 0;
    const int rootPageOffset = MagicNumberOffset + 4;
    const int usedRecordOffset = rootPageOffset + 4;
    const int avaliableRecordOffset = usedRecordOffset + 4;
    const int LinkHeadPageOffset = avaliableRecordOffset + 4;
    const int LinkHeadPageOffOffset = LinkHeadPageOffset + 2;
    struct BTree {
        PageDB::Scheduler* pgdb;
        PageDB::File* file;
        PageDB::PageWriteSession entrySession;
        BTree(PageDB::Scheduler* _pgdb, const std::string& fn);
        std::pair<bool, Value> find(int key) {
            return find(Key(key));
        }
        std::pair<bool, Value> find(const std::string& key) {
            return find(Key(key));
        }
        bool set(int key, Value value, bool force = false) {
            return set(Key(key), value, force);
        }
        bool set(const std::string& key, Value value, bool force = false) {
            return set(Key(key), value, force);
        }
        bool remove(int key) {
            return remove(Key(key));
        }
        bool remove(const std::string& key) {
            return remove(Key(key));
        }
        BTreeIterator writable_begin();
        BTreeIterator writable_end();
        BTreeConstIterator begin();
        BTreeConstIterator end();
    private:
        std::pair<bool, Value> find(const Key& key);
        bool set(const Key& key, Value value, bool force = false);
        bool remove(const Key& key);
        void trace(const Key& key, std::vector<int>* tr, bool& found, PageDB::Location& loc);
        void initBTree();
        void insertCore(Key key, std::vector<int>& trace, PageDB::Location loc);
        void removeCore(Key key, std::vector<int>& trace);
        int& magicNumber() {
            return *(int*)(entrySession.page().buf + MagicNumberOffset);
        }
        int& rootPage() {
            return *(int*)(entrySession.page().buf + rootPageOffset);
        }
        int& usedRecord() {
            return *(int*)(entrySession.page().buf + usedRecordOffset);
        }
        int& avalibleRecord() {
            return *(int*)(entrySession.page().buf + avaliableRecordOffset);
        }
        int& linkHeadPage() {
            return *(int*)(entrySession.page().buf + LinkHeadPageOffset);
        }
        int& linkHeadOffset() {
            return *(int*)(entrySession.page().buf + LinkHeadPageOffOffset);
        }
    };
}
#endif
