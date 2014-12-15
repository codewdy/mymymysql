#ifndef WDY_9123193456_BTREE
#define WDY_9123193456_BTREE

#include "PageDB/scheduler.hpp"
#include "PageDB/iterator.hpp"

namespace BTree {
    typedef std::pair<int, int> ValueType;
    struct Key {
        int hash1, hash2, hash3;
        Key(const std::string& _key);
        Key(int _key);
        Key(int _hash1, int _hash2, int _hash3);
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
        ValueType value();
    };
    struct BTreeIterator : public PageDB::Iterator {
        //TODO
        BTreeIterator(PageDB::Scheduler* _pgdb, const std::string& fn);
        BTreeIterator(PageDB::Scheduler* _pgdb, PageDB::File* _file);
        ValueType value();
    };
    struct BTree {
        BTree(PageDB::Scheduler* pgdb, const std::string& fn);
        std::pair<bool, ValueType> find(int key) {
            return find(Key(key));
        }
        std::pair<bool, ValueType> find(const std::string& key) {
            return find(Key(key));
        }
        bool set(int key, ValueType value, bool force = false) {
            return set(Key(key), value, force);
        }
        bool set(const std::string& key, ValueType value, bool force = false) {
            return set(Key(key), value, force);
        }
        bool remove(int key) {
            return remove(Key(key));
        }
        bool remove(const std::string& key) {
            return remove(Key(key));
        }
    private:
        std::pair<bool, ValueType> find(const Key& key);
        bool set(const Key& key, ValueType value, bool force = false);
        bool remove(const Key& key);
    };
}

#endif
