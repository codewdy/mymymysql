#ifndef WDY_0192103485_BTREE_BASE
#define WDY_0192103485_BTREE_BASE
#include "PageDB/file.hpp"
#include "Hash.hpp"
namespace BTree {
    typedef PageDB::Location Value;
    inline bool operator<(const Key& lhs, const Key& rhs) {
        if (lhs.hash1 == rhs.hash1 && lhs.hash2 == rhs.hash2)
            return lhs.hash3 < rhs.hash3;
        if (lhs.hash1 == rhs.hash1)
            return lhs.hash2 < rhs.hash2;
        return lhs.hash1 < rhs.hash1;
    }
    inline bool operator>(const Key& lhs, const Key& rhs) {
        return rhs < lhs;
    }
    inline bool operator<=(const Key& lhs, const Key& rhs) {
        return !(rhs > lhs);
    }
    inline bool operator>=(const Key& lhs, const Key& rhs) {
        return !(rhs < lhs);
    }
    inline bool operator==(const Key& lhs, const Key& rhs) {
        return lhs.hash1 == rhs.hash1 && lhs.hash2 == rhs.hash2 && lhs.hash3 == rhs.hash3;
    }
    inline bool operator!=(const Key& lhs, const Key& rhs) {
        return !(rhs == lhs);
    }
    const int MinChild = PageDB::PAGE_SIZE / 16 / 2 - 1;
    struct Node {
        int size;
        struct Child {
            Key less;
            PageDB::Location loc;
        } children[MinChild * 2];
        int findIndex(const Key& key) const;
        bool InsertAndSplit(Key key, PageDB::Location loc, Node& splitNode);
        void Insert(Key key, PageDB::Location loc);
        void Remove(int idx);
        void Merge(const Node& right);
    };
    struct Information {
        Key key;
        Value value;
        PageDB::Location next, prev;
    };
}
#endif
