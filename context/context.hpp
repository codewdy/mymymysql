#ifndef WDY_9203812930_CONTEXT
#define WDY_9203812930_CONTEXT

#include "TypeDB/object.hpp"
#include "TypeDB/table.hpp"
#include "PageDB/lru_scheduler.hpp"
#include "PageDB/scheduler.hpp"

namespace Context {
    static const std::string DefaultDB = "test";
    static const std::string DBFilename = "DB.DBX";
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
        Context(PageDB::Scheduler* _pgdb) : pgdb(_pgdb), dbName(DefaultDB) {Init();}
        void InitTable(const std::string& tblName, const TypeDB::TableDesc& desc) const ;
        TypeDB::TableDesc GetTableDesc(const std::string& tblName) const;
        void DropTable(const std::string& tblName) const;
        TypeDB::Table GetTable(const std::string& tblName) const;
        void AssertTable(const std::string& tblName) const;
        void Insert(const std::string& tblName, const TypeDB::Table& tbl) const;
        void Update(const std::string& tblName, const TypeDB::Table& tbl) const;
        void Delete(const std::string& tblName, const TypeDB::Table& tbl) const;
        std::vector<std::string> ReadDB() const;
        void WriteDB(const std::vector<std::string>& info) const;
        std::vector<std::string> ReadDB(const std::string& fn) const;
        void WriteDB(const std::string& fn, const std::vector<std::string>& info) const;
        bool dbNewTable(const std::string& tblName) const;
        bool dbRemoveTable(const std::string& tblName) const;
        void UseDB(const std::string& _dbName);
        void CreateDB(const std::string& _dbName);
        void DropDB(const std::string& _dbName);
        void Init();
    };
}
#endif
