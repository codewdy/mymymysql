#include "tblSelector.hpp"

namespace TypeDB {
    Table RawTableSelector::getTable(const Context::Context& ctx) {
        ctx.AssertTable(tblname);
        return ctx.GetTable(tblname);
    }
}
