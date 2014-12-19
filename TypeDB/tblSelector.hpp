#ifndef WDY_9182102932_TBLSELECTOR
#define WDY_9182102932_TBLSELECTOR
#include "table.hpp"
#include "context/context.hpp"

namespace TypeDB {
    struct TableSelector {
        virtual Table getTable(Context::Context& ctx) = 0;
        virtual ~TableSelector();
    };
    struct RawTableSelector : TableSelector{
        std::string tblname;
        RawTableSelector(std::string _tblname) : tblname(_tblname) {}
        virtual Table getTable(Context::Context& ctx);
    };
}

#endif
