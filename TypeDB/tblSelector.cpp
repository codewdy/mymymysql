#include "tblSelector.hpp"

namespace TypeDB {
    Table RawTableSelector::getTable(Context::Context& ctx) {
        return ctx.GetTable(tblname);
    }
}
