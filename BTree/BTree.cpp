#include "BTree.hpp"
#include <vector>
#include "Utils/writeFile.hpp"
#include "Utils/bufOp.hpp"

namespace BTree {


    BTree::BTree(PageDB::Scheduler* _pgdb, const std::string& fn)
        : pgdb(_pgdb), file(pgdb->OpenFile(fn)),
        entrySession(pgdb->GetWriteSession(file, file->entryPageID)) {
        if (magicNumber() != MagicNumber) {
            initBTree();
        }
    }
    std::pair<bool, Value> BTree::find(const Key& key) {
        std::pair<bool, Value> ret;
        bool found;
        PageDB::Location loc;
        trace(key, nullptr, found, loc);
        ret.first = found;
        if (found) {
            PageDB::PageSession session = pgdb->GetSession(file, loc.Page);
            const Information* x;
            x = reinterpret_cast<const Information*>(session.buf() + loc.Offset);
            ret.second = x->value;
        }
        return ret;
    }
    bool BTree::set(const Key& key, Value value, bool force) {
        std::vector<int> tr;
        bool found;
        PageDB::Location loc;
        trace(key, &tr, found, loc);
        if (!found && !force)
            return false;
        Information* info;
        if (found) {
            PageDB::PageWriteSession session = pgdb->GetWriteSession(file, loc.Page);
            info = reinterpret_cast<Information*>(session.buf() + loc.Offset);
            info->key = key;
            info->value = value;
            return true;
        }
        char buf[sizeof(Information)];
        info = reinterpret_cast<Information*>(buf);
        info->key = key;
        info->value = value;
        auto locX = Utils::writeFile(pgdb, file, buf, sizeof(Information));
        insertCore(key, tr, locX);
        return true;
    }
    bool BTree::remove(const Key& key) {
        std::vector<int> tr;
        bool found;
        PageDB::Location loc;
        trace(key, &tr, found, loc);
        if (!found)
            return false;
        removeCore(key, tr);
        return true;
    }
    void BTree::trace(const Key& key, std::vector<int>* tr, bool& found, PageDB::Location& loc) {
        const Node* node;
        int currentPage = rootPage();
        while (true) {
            if (tr)
                tr->push_back(currentPage);
            PageDB::PageSession session = pgdb->GetSession(file, currentPage);
            node = reinterpret_cast<const Node*>(session.buf());
            if (node->size == 0) {
                found = false;
                break;
            }
            int idx = node->findIndex(key);
            auto locX = node->children[idx].loc;
            if (locX.Page != 0) {
                if (node->children[idx].less == key) {
                    found = true;
                    loc = locX;
                    break;
                } else {
                    found = false;
                    break;
                }
            } else {
                currentPage = locX.Offset;
            }
        }
    }
    void BTree::initBTree() {
        magicNumber() = MagicNumber;
        int rtPage = file->newPage();
        int infoPage = file->newPage();
        file->eof.Page = infoPage;
        file->eof.Offset = 0;
        file->writebackFileHeaderCore();
        rootPage() = rtPage;
        usedRecord() = 0;
        avalibleRecord() = 0;
        linkHeadPage() = infoPage;
        linkHeadOffset() = 0;
        entrySession.flush();
    }
    void BTree::insertCore(Key key, std::vector<int>& trace, PageDB::Location loc) {
        Node *currentNode, *splitNode;
        int sp = trace.size() - 1;
        for(; sp > -1; sp--) {
            PageDB::PageWriteSession session1 = pgdb->GetWriteSession(file, trace[sp]);
            currentNode = reinterpret_cast<Node*>(session1.buf());
            if (currentNode->size != MinChild * 2) {
                currentNode->Insert(key, loc);
                break;
            }
            int newPage = file->newPage();
            PageDB::PageWriteSession session2 = pgdb->GetWriteSession(file, newPage);
            splitNode = reinterpret_cast<Node*>(session2.buf());
            currentNode->InsertAndSplit(key, loc, *splitNode);
            loc.Page = 0;
            loc.Offset = newPage;
            key = splitNode->children[0].less;
        }
        if (sp == -1) {
            int newPage = file->newPage();
            PageDB::PageWriteSession session = pgdb->GetWriteSession(file, newPage);
            currentNode = reinterpret_cast<Node*>(session.buf());
            currentNode->size = 2;
            currentNode->children[0].less = Key(INT_MIN, INT_MIN, INT_MIN);
            currentNode->children[0].loc = PageDB::Location(0, trace[0]);
            currentNode->children[1].less = key;
            currentNode->children[1].loc = loc;
            rootPage() = newPage;
            entrySession.flush();
        }
    }
    void BTree::removeCore(Key key, std::vector<int>& trace) {
        Node *parent;
        PageDB::PageWriteSession parentSession = pgdb->GetWriteSession(file, trace.back());
        parent = reinterpret_cast<Node*>(parentSession.buf());
        int sp;
        int idx = parent->findIndex(key);
        for (sp = trace.size() - 1; sp > 0; sp--) {
            Node* child = parent;
            child->Remove(idx);
            if (child->size >= MinChild) {
                break;
            }
            PageDB::PageWriteSession childSession = std::move(parentSession);
            parentSession = pgdb->GetWriteSession(file, trace[sp - 1]);
            parent = reinterpret_cast<Node*>(parentSession.buf());
            idx = parent->findIndex(child->children[0].less);
            if (idx > 0) {
                //Left
                PageDB::PageWriteSession siblingSession = pgdb->GetWriteSession(file, parent->children[idx - 1].loc.Offset);
                Node* sibling = reinterpret_cast<Node*>(siblingSession.buf());
                if (sibling->size > MinChild) {
                    auto item = sibling->children[sibling->size - 1];
                    child->Insert(item.less, item.loc);
                    sibling->Remove(sibling->size - 1);
                    parent->children[idx].less = child->children[0].less;
                    break;
                }
                //Merge
                sibling->Merge(*child);
                childSession.Release();
                pgdb->RemovePage(file, trace[sp]);
            } else {
                //Right
                PageDB::PageWriteSession siblingSession = pgdb->GetWriteSession(file, parent->children[idx + 1].loc.Offset);
                Node* sibling = reinterpret_cast<Node*>(siblingSession.buf());
                if (sibling->size > MinChild) {
                    auto item = sibling->children[0];
                    child->Insert(item.less, item.loc);
                    sibling->Remove(0);
                    parent->children[idx + 1].less = sibling->children[0].less;
                    break;
                }
                //Merge
                child->Merge(*sibling);
                siblingSession.Release();
                pgdb->RemovePage(file, parent->children[idx + 1].loc.Offset);
                idx++;
            }
        }
        if (sp == 0) {
            parent->Remove(idx);
            if (parent->size == 1 && parent->children[0].loc.Page == 0) {
                rootPage() = parent->children[0].loc.Offset;
                parentSession.Release();
                entrySession.flush();
            }
        }
    }
    const Information& BTreeConstIterator::Info() {
        const Information* info = reinterpret_cast<const Information*>(Get());
        return *info;
    }
    Information& BTreeIterator::Info() {
        Information* info = reinterpret_cast<Information*>(Get());
        return *info;
    }
    Value BTreeConstIterator::value() {
        return Info().value;
    }
    Value& BTreeIterator::value() {
        return Info().value;
    }
    PageDB::Location BTreeConstIterator::NextLocation() {
        return Info().next;
    }
    PageDB::Location BTreeIterator::NextLocation() {
        return Info().next;
    }
}
