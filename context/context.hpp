#ifndef WDY_9203812930_CONTEXT
#define WDY_9203812930_CONTEXT

#include "TypeDB/object.hpp"
#include "TypeDB/table.hpp"
#include "PageDB/lru_scheduler.hpp"
#include "PageDB/scheduler.hpp"

namespace Context {
    typedef PageDB::LRU_Scheduler DefaultScheduler;
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
        Context() : pgdb(new DefaultScheduler), dbName(DefaultDB) {}
        void InitTable(const std::string& tblName, const TypeDB::TableDesc& desc) const;
        TypeDB::TableDesc GetTableDesc(const std::string& tblName) const;
        void DropTable(const std::string& tblName) const;
        TypeDB::Table GetTable(const std::string& tblName) const;
        std::pair<int, int> InsertCore(const char* beg, const char* end) const;
        void Update(const std::string& tblName, const TypeDB::Table tbl) const;
        void Delete(const std::string& tblName, const TypeDB::Table tbl) const;
        void ChangeDB(const std::string& _dbName) {
            dbName = _dbName;
        }
    };
}
#endif
