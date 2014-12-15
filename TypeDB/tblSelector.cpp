#include "tblSelector.hpp"

namespace TypeDB {
    Table RawTableSelector::getTable(const Context::Context& ctx) {
        return ctx.GetTable(tblname);
    }
}
