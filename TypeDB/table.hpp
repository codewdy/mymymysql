#ifndef WDY_9218312393_TABLE
#define WDY_9218312393_TABLE
#include "type.hpp"
#include <vector>
#include <string>
#include "Exception/Exception.hpp"

namespace TypeDB {
    struct ColDesc {
        std::string tbl, name;
        Type* type;
        std::string toString() {
            if (tbl.empty())
                return name;
            else
                return tbl + '.' + name;
        }
    };
    struct Row {
        std::vector<pObject> objs;
        static Row merge(const Row& lhs, const Row& rhs);
    };
    struct TableDesc {
        std::vector<ColDesc> descs;
        std::size_t primaryIndex;
        int getIndex(const std::string& tbl, const std::string& name, bool force = false) const;
        pObject getObject(const std::vector<Row*>& rows, const std::string& tbl, const std::string& name, bool force = false) const;
        pObject getObject(const Row& rows, const std::string& tbl, const std::string& name, bool force = false) const;
        pObject getPrimary(const Row& row) const;
        static TableDesc merge(const TableDesc& lhs, const TableDesc& rhs);
        bool Test(const Row& row) const;
    };
    struct Table {
        TableDesc desc;
        std::vector<Row> rows;
        std::vector<pObject> getVec(const std::string& tbl, const std::string& name) const;
    };
}

#endif
