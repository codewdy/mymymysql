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
    struct TableDesc {
        std::vector<ColDesc> descs;
        std::size_t primaryIndex;
        std::size_t getIndex(const std::string& tbl, const std::string& name) const;
        std::size_t primaryKey() const;
        static TableDesc merge(const TableDesc& lhs, const TableDesc& rhs);
    };
    struct Row {
        std::vector<pObject> objs;
        TableDesc* desc;
        pObject getObj(const std::string& tbl, const std::string& name) const;
        pObject getPrimary() const;
        static Row merge(const Row& lhs, const Row& rhs);
    };
    struct Table {
        TableDesc desc;
        std::vector<Row> rows;
        std::vector<pObject> getVec(const std::string& tbl, const std::string& name) const;
    };
}

#endif
