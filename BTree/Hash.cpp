#include "Hash.hpp"
namespace BTree {
    static const int p1 = 98765431, p2 = 10016957, p3 = 57885161;
    Key::Key(const std::string& _key) {
        hash1 = hash2 = hash3 = 0;
        for (auto c : _key) {
            hash1 = hash1 * p1 + c;
            hash2 = hash2 * p2 + c;
            hash3 = hash3 * p3 + c;
        }
    }
    Key::Key(int _key) {
        hash1 = hash2 = hash3 = _key;
    }
    Key::Key(int _hash1, int _hash2, int _hash3) :
        hash1(_hash1), hash2(_hash2), hash3(_hash3) {}
}
