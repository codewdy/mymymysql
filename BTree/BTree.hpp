#ifndef WDY_9123193456_BTREE
#define WDY_9123193456_BTREE

#include "PageDB/scheduler.hpp"
#include "PageDB/iterator.hpp"

namespace BTree {
    typedef std::pair<int, int> ValueType;
    struct BTree {
        BTree(PageDB::Scheduler* pgdb, const std::string& fn);
        std::pair<bool, ValueType> find(int key);
        std::pair<bool, ValueType> find(const std::string& key);
        bool set(int key, ValueType value, bool force = false);
        bool set(const std::string& key, ValueType value, bool force = false);
        bool remove(int key);
        bool remove(const std::string& key);
    private:
        std::pair<bool, ValueType> find(int hash1, int hash2, int hash3);
        bool set(int hash1, int hash2, int hash3, ValueType value, bool force = false);
        bool remove(int hash1, int hash2, int hash3);
    };
    struct BTreeConstIterator : public PageDB::ConstIterator {
        //TODO
        BTreeConstIterator(PageDB::Scheduler* _pgdb, const std::string& fn);
        BTreeConstIterator(PageDB::Scheduler* _pgdb, PageDB::File* _file);
        ValueType value();
    };
}

#endif
