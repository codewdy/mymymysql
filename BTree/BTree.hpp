#ifndef WDY_9123193456_BTREE
#define WDY_9123193456_BTREE

#include "PageDB/scheduler.hpp"
#include "PageDB/iterator.hpp"
#include <vector>

namespace BTree {
    typedef PageDB::Location Value;
    struct Key {
        int hash1, hash2, hash3;
        Key(const std::string& _key);
        Key(int _key);
        Key(int _hash1, int _hash2, int _hash3);
        Key(){}
    };
    bool operator<(const Key& lhs, const Key& rhs) {
        if (lhs.hash1 == rhs.hash1 && lhs.hash2 == rhs.hash2)
            return lhs.hash3 < rhs.hash3;
        if (lhs.hash1 == rhs.hash1)
            return lhs.hash2 < rhs.hash2;
        return lhs.hash1 < rhs.hash1;
    }
    bool operator>(const Key& lhs, const Key& rhs) {
        return rhs < lhs;
    }
    bool operator<=(const Key& lhs, const Key& rhs) {
        return !(rhs > lhs);
    }
    bool operator>=(const Key& lhs, const Key& rhs) {
        return !(rhs < lhs);
    }
    bool operator==(const Key& lhs, const Key& rhs) {
        return lhs.hash1 == rhs.hash1 && lhs.hash2 == rhs.hash2 && lhs.hash3 == rhs.hash3;
    }
    bool operator!=(const Key& lhs, const Key& rhs) {
        return !(rhs == lhs);
    }
    struct BTreeConstIterator : public PageDB::ConstIterator {
        //TODO
        BTreeConstIterator(PageDB::Scheduler* _pgdb, const std::string& fn);
        BTreeConstIterator(PageDB::Scheduler* _pgdb, PageDB::File* _file);
        Value value();
    };
    struct BTreeIterator : public PageDB::Iterator {
        //TODO
        BTreeIterator(PageDB::Scheduler* _pgdb, const std::string& fn);
        BTreeIterator(PageDB::Scheduler* _pgdb, PageDB::File* _file);
        Value value();
    };
    const int rootPageOffset = 0;
    const int usedRecordOffset = rootPageOffset + 4;
    const int avaliableRecordOffset = usedRecordOffset + 4;
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
    private:
        std::pair<bool, Value> find(const Key& key);
        bool set(const Key& key, Value value, bool force = false);
        bool remove(const Key& key);
        void trace(const Key& key, std::vector<int>* tr, bool& found, PageDB::Location& loc);
        void insertCore(Key key, std::vector<int>& trace, PageDB::Location loc);
        void removeCore(Key key, std::vector<int>& trace);
        int& rootPage() {
            return *(int*)(entrySession.page().buf + rootPageOffset);
        }
        int& usedRecord() {
            return *(int*)(entrySession.page().buf + usedRecordOffset);
        }
        int& avalibleRecord() {
            return *(int*)(entrySession.page().buf + avaliableRecordOffset);
        }
    };
}
#endif
