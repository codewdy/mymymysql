#include "context.hpp"
#include "utils/bufOp.hpp"
#include "BTree/BTree.hpp"
#include "Utils/writeFile.hpp"

namespace Context {
    typedef std::pair<TypeDB::Type*, std::string> colTypeDesc;
    enum TypeEnum : Utils::byte {
        IntEnum,
        StringEnum,
    };
    static TypeDB::Type* readType(const char*& buf) {
        switch (Utils::readByte(buf)) {
            case IntEnum:
                return TypeDB::intType;
            case StringEnum:
                return TypeDB::stringType;
            default:
                //TODO
                throw "Not Imp";
        }
    }
    static void writeType(char*& buf, TypeDB::Type* desc) {
        if (desc == TypeDB::intType) {
            Utils::writeByte(buf, IntEnum);
        } else if (desc == TypeDB::stringType) {
            Utils::writeByte(buf, StringEnum);
        } else {
            //TODO
            throw "Not Imp";
        }
    }
    void Context::InitTable(const std::string& tblName, const TypeDB::TableDesc& desc) {
        if (!dbNewTable(tblName)) {
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
        file->eof = PageDB::Location(file->newPage(), 0);
        file->writebackFileHeaderCore();
    }
    TypeDB::TableDesc Context::GetTableDesc(const std::string& tblName) {
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
    void Context::DropTable(const std::string& tblName) {
        if (!dbRemoveTable(tblName)) {
            //TODO: throw a exception
            throw "Not Imp";
        }
    }
    TypeDB::Table Context::GetTable(const std::string& tblName) {
        TypeDB::Table ret;
        ret.desc = GetTableDesc(tblName);
        BTree::BTree btree(pgdb, tblidxFileName(tblName));
        PageDB::ConstIterator pgiter(pgdb, pgdb->OpenFile(tblFileName(tblName)));
        auto iter = btree.begin(), end = btree.end();
        for (; iter != end; iter.Next()) {
            auto v = iter.value();
            std::cout << iter.Info().key.hash1 << std::endl;
            pgiter.Goto(v);
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
    static char* WriteRow(char* buf, const TypeDB::Row& row) {
        char* org_buf = buf;
        Utils::jumpWord(buf);
        for (auto item : row.objs)
            item->write(buf);
        Utils::writeWord(org_buf, buf - org_buf);
        return buf;
    }
    void Context::Insert(const std::string& tblName, const TypeDB::Table tbl) {
        PageDB::File* tblFile = pgdb->OpenFile(tblFileName(tblName));
        BTree::BTree btree(pgdb, tblidxFileName(tblName));
        PageDB::Iterator iter(pgdb, tblFile);
        char* writeBuf = new char[PageDB::PAGE_SIZE];
        for (const TypeDB::Row& row : tbl.rows) {
            char* eob = WriteRow(writeBuf, row);
            auto loc = Utils::writeFile(pgdb, tblFile, writeBuf, eob - writeBuf);
            std::cout << row.getPrimary()->toString() << std::endl;
            btree.set(row.getPrimary()->hash(), loc, true);
        }
        delete [] writeBuf;
    }
    void Context::Update(const std::string& tblName, const TypeDB::Table tbl) {
        PageDB::File* tblFile = pgdb->OpenFile(tblFileName(tblName));
        BTree::BTree btree(pgdb, tblidxFileName(tblName));
        PageDB::Iterator iter(pgdb, tblFile);
        char* writeBuf = new char[PageDB::PAGE_SIZE];
        for (const TypeDB::Row& row : tbl.rows) {
            auto loc = btree.find(row.getPrimary()->hash());
            if (loc.first) {
                throw "Not Imp";
            }
            iter.Goto(loc.second);
            char* buf = iter.Get();
            int size = Utils::readWord(buf);
            char* eob = WriteRow(writeBuf, row);
            if (eob - writeBuf > size) {
                auto new_loc = Utils::writeFile(pgdb, tblFile, writeBuf, eob - writeBuf);
                btree.set(row.getPrimary()->hash(), new_loc, true);
            } else {
                memcpy(buf, writeBuf, eob - writeBuf);
            }
        }
        delete [] writeBuf;
    }
    void Context::Delete(const std::string& tblName, const TypeDB::Table tbl) {
        BTree::BTree btree(pgdb, tblidxFileName(tblName));
        for (const TypeDB::Row& row : tbl.rows) {
            btree.remove(row.getPrimary()->hash());
        }
    }
    std::vector<std::string> Context::ReadDB() {
        PageDB::File* dbFile = pgdb->OpenFile(dbFileName());
        PageDB::PageSession session = pgdb->GetSession(dbFile, dbFile->entryPageID);
        const char* buf = session.buf();
        int size = Utils::readInt(buf);
        std::vector<std::string> ret;
        for (int i = 0; i < size; i++)
            ret.push_back(Utils::readString(buf));
        return ret;
    }
    void Context::WriteDB(const std::vector<std::string>& info) {
        PageDB::File* dbFile = pgdb->OpenFile(dbFileName());
        PageDB::PageWriteSession session = pgdb->GetWriteSession(dbFile, dbFile->entryPageID);
        char* buf = session.buf();
        Utils::writeInt(buf, info.size());
        for (auto item : info)
            Utils::writeString(buf, item);
    }
    bool Context::dbNewTable(const std::string& tblName) {
        auto x = ReadDB();
        for (std::size_t i = 0; i < x.size(); i++) {
            if (x[i] == tblName) {
                return false;
            }
        }
        x.push_back(tblName);
        WriteDB(x);
        return true;
    }
    bool Context::dbRemoveTable(const std::string& tblName) {
        auto x = ReadDB();
        std::size_t p;
        for (p = 0; p < x.size(); p++) {
            if (x[p] == tblName) {
                break;
            }
        }
        if (p == x.size()) {
            return false;
        }
        for (std::size_t i = p; i + 1 < x.size(); i++)
            x[i] = x[i + 1];
        x.pop_back();
        WriteDB(x);
        return true;
    }
}
