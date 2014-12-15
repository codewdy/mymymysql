#include "context.hpp"
#include "utils/bufOp.hpp"
#include "BTree/BTree.hpp"

namespace Context {
    typedef std::pair<TypeDB::Type*, std::string> colTypeDesc;
    enum TypeEnum {
        IntEnum,
        StringEnum,
    };
    static TypeDB::Type* readType(const char*& buf) {
        switch (Utils::readByte(buf)) {
            case IntEnum:
                return TypeDB::intType;
            case StringEnum:
                return TypeDB::stringType;
        }
        return nullptr;
    }
    static void writeType(char*& buf, TypeDB::Type* desc) {
        if (desc == TypeDB::intType) {
            Utils::writeByte(buf, IntEnum);
        } else if (desc == TypeDB::stringType) {
            Utils::writeByte(buf, StringEnum);
        }
    }
    static bool newTable(const std::string& dbFile, const std::string& tblName) {
        //TODO
        throw "Not Imp";
    }
    static bool removeTable(const std::string& dbFile, const std::string& tblName) {
        //TODO
        throw "Not Imp";
    }
    void Context::InitTable(const std::string& tblName, const TypeDB::TableDesc& desc) const {
        if (!newTable(dbFileName(), tblName)) {
            //TODO: throw a exception
            throw "Not Imp";
        }
        auto file = pgdb->OpenFile(tblFileName(tblName));
        auto x = pgdb->GetWriteSession(file, file->entryPageID);
        auto buf = x.buf();
        Utils::writeInt(buf, 1);
        Utils::writeInt(buf, 0);
        Utils::writeWord(buf, desc.descs.size());
        Utils::writeWord(buf, desc.primaryIndex);
        for (std::size_t i = 0; i < desc.descs.size(); i++) {
            writeType(buf, desc.descs[i].type);
            Utils::writeString(buf, desc.descs[i].name);
        }
    }
    TypeDB::TableDesc Context::GetTableDesc(const std::string& tblName) const {
        auto file = pgdb->OpenFile(tblFileName(tblName));
        auto x = pgdb->GetSession(file, file->entryPageID);
        auto buf = x.buf();
        Utils::jumpInt(buf);
        Utils::jumpInt(buf);
        auto sz = Utils::readWord(buf);
        TypeDB::TableDesc ret;
        ret.primaryIndex = Utils::readWord(buf);
        ret.descs.resize(sz);
        for (int i = 0; i < sz; i++) {
            ret.descs[i].type = readType(buf);
            ret.descs[i].tbl = tblName;
            ret.descs[i].name = Utils::readString(buf);
        }
        return std::move(ret);
    }
    void Context::DropTable(const std::string& tblName) const {
        if (!removeTable(dbFileName(), tblName)) {
            //TODO: throw a exception
            throw "Not Imp";
        }
    }
    TypeDB::Table Context::GetTable(const std::string& tblName) const {
        TypeDB::Table ret;
        ret.desc = GetTableDesc(tblName);
        BTree::BTreeConstIterator iter(pgdb, tblidxFileName(tblName));
        PageDB::ConstIterator pgiter(pgdb, tblFileName(tblName));
        for (; !iter.End(); iter.Next()) {
            auto v = iter.value();
            pgiter.Goto(v.first, v.second);
            auto buf = pgiter.Get();
            TypeDB::Row row;
            row.desc = &ret.desc;
            row.objs.resize(row.desc->descs.size());
            Utils::jumpWord(buf);
            for (std::size_t i = 0; i < row.desc->descs.size(); i++)
                row.objs[i] = row.desc->descs[i].type->CreateAndJump(buf);
            ret.rows.push_back(std::move(row));
        }
        return ret;
    }
    std::pair<int, int> Context::InsertCore(const char* beg, const char* end) const {
        //TODO
        throw "Not Imp";
    }
    static char* WriteRow(char* buf, const TypeDB::Row& row) {
        //TODO
        throw "Not Imp";
    }
    static void WriteBTree(BTree::BTree& btree, TypeDB::Object* key, int pageid, int offset, bool force = false) {
        //TODO
        throw "Not Imp";
    }
    static std::pair<int, int> ReadBTree(BTree::BTree& btree, TypeDB::Object* key) {
        //TODO
        throw "Not Imp";
    }
    static void RemoveBTree(BTree::BTree& btree, TypeDB::Object* key) {
        //TODO
        throw "Not Imp";
    }
    void Context::Update(const std::string& tblName, const TypeDB::Table tbl) const {
        BTree::BTree btree(pgdb, tblidxFileName(tblName));
        PageDB::Iterator iter(pgdb, tblFileName(tblName));
        char* writeBuf = new char[PageDB::PAGE_SIZE];
        for (const TypeDB::Row& row : tbl.rows) {
            auto loc = ReadBTree(btree, row.getPrimary());
            iter.Goto(loc.first, loc.second);
            char* buf = iter.Get();
            int size = Utils::readWord(buf);
            char* eob = WriteRow(writeBuf, row);
            if (eob - writeBuf > size) {
                auto new_loc = InsertCore(writeBuf, eob);
                WriteBTree(btree, row.getPrimary(), new_loc.first, new_loc.second, true);
            } else {
                memcpy(buf, writeBuf, eob - writeBuf);
            }
        }
        delete [] writeBuf;
    }
    void Context::Delete(const std::string& tblName, const TypeDB::Table tbl) const {
        BTree::BTree btree(pgdb, tblidxFileName(tblName));
        for (const TypeDB::Row& row : tbl.rows) {
            RemoveBTree(btree, row.getPrimary());
        }
    }
}
