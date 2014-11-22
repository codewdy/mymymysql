#ifndef WDY_9218312393_TABLE
#define WDY_9218312393_TABLE
#include "type.hpp"
#include <vector>
#include <string>

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
    typedef std::vector<ColDesc> TableDesc;
    struct Row {
        std::vector<pObject> objs;
        TableDesc* desc;
    };
    struct Table {
        TableDesc desc;
        std::vector<Row> rows;
    };
}

#endif
