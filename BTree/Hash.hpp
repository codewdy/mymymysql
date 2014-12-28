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
    inline bool operator<(const Key& lhs, const Key& rhs) {
        if (lhs.hash1 == rhs.hash1 && lhs.hash2 == rhs.hash2)
            return lhs.hash3 < rhs.hash3;
        if (lhs.hash1 == rhs.hash1)
            return lhs.hash2 < rhs.hash2;
        return lhs.hash1 < rhs.hash1;
    };
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
}
#endif
