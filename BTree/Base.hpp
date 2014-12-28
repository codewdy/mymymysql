#ifndef WDY_0192103485_BTREE_BASE
#define WDY_0192103485_BTREE_BASE
#include "PageDB/file.hpp"
#include "Hash.hpp"
namespace BTree {
    typedef PageDB::Location Value;
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
