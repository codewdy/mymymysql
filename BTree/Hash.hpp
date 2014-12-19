#ifndef WDY_0129109765_BTREE_HASH
#define WDY_0129109765_BTREE_HASH
#include <string>
namespace BTree {
    struct Key {
        int hash1, hash2, hash3;
        Key(const std::string& _key);
        Key(int _key);
        Key(int _hash1, int _hash2, int _hash3);
        Key(){}
    };
}
#endif
