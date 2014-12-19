#include "Base.hpp"

namespace BTree {
    int Node::findIndex(const Key& key) const {
        int sb = 0, se = size - 1;
        while (sb < se) {
            int sm = (sb + se) / 2 + 1;
            if (key < children[sm].less)
                se = sm - 1;
            else
                sb = sm;
        }
        return sb;
    }
    bool Node::InsertAndSplit(Key key, PageDB::Location loc, Node& splitNode) {
        if (size == MinChild * 2) {
            size = MinChild;
            splitNode.size = MinChild;
            for (int i = 0; i < MinChild; i++)
                splitNode.children[i] = children[i + MinChild];
            if (key < splitNode.children[0].less)
                Insert(key, loc);
            else
                splitNode.Insert(key, loc);
            return true;
        }
        Insert(key, loc);
        return false;
    }
    void Node::Insert(Key key, PageDB::Location loc) {
        int i;
        for (i = size; i > 0; i--) {
            if (key < children[i - 1].less)
                children[i] = children[i - 1];
            else
                break;
        }
        children[i].less = key;
        children[i].loc = loc;
        size++;
    }
    void Node::Remove(int idx) {
        int p = idx;
        size--;
        for (int i = p; i < size; i++)
            children[i] = children[i + 1];
    }
    void Node::Merge(const Node& right) {
        for (int i = 0; i < right.size; i++)
            children[size + i] = right.children[i];
        size += right.size;
    }
}
