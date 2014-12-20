#ifndef WDY_9203812930_CONTEXT
#define WDY_9203812930_CONTEXT

#include "TypeDB/object.hpp"
#include "TypeDB/table.hpp"
#include "PageDB/lru_scheduler.hpp"
#include "PageDB/scheduler.hpp"

namespace Context {
    static const std::string DefaultDB = "test";
    struct Context {
        PageDB::Scheduler* pgdb;
        std::string dbName;
        std::string tblFileName(const std::string& tblName) const {
            return dbName + ":" + tblName + ".db";
        }
        std::string tblidxFileName(const std::string& tblName) const {
            return dbName + ":" + tblName + ".idx";
        }
        std::string dbFileName() const {
            return dbName + ".db";
        }
        Context(PageDB::Scheduler* _pgdb) : pgdb(_pgdb), dbName(DefaultDB) {}
        void InitTable(const std::string& tblName, const TypeDB::TableDesc& desc);
        TypeDB::TableDesc GetTableDesc(const std::string& tblName);
        void DropTable(const std::string& tblName);
        TypeDB::Table GetTable(const std::string& tblName);
        void Insert(const std::string& tblName, const TypeDB::Table tbl);
        void Update(const std::string& tblName, const TypeDB::Table tbl);
        void Delete(const std::string& tblName, const TypeDB::Table tbl);
        std::vector<std::string> ReadDB();
        void WriteDB(const std::vector<std::string>& info);
        bool dbNewTable(const std::string& tblName);
        bool dbRemoveTable(const std::string& tblName);
        void ChangeDB(const std::string& _dbName) {
            dbName = _dbName;
        }
    };
}
#endif
