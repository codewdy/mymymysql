#include "table.hpp"
#include "Exception/Exception.hpp"

namespace TypeDB {
    std::size_t TableDesc::getIndex(const std::string& tbl, const std::string& name) const {
        int result = -1;
        if (tbl == "") {
            for (std::size_t i = 0; i < descs.size(); i++) {
                if (descs[i].name == name) {
                    if (result == -1)
                        result = i;
                    else
                        RAISE(ColumnMultiFound, name);
                }
            }
        } else {
            for (std::size_t i = 0; i < descs.size(); i++) {
                if (descs[i].tbl == tbl && descs[i].name == name) {
                    result = i;
                    break;
                }
            }
        }
        if (result == -1)
            RAISE(ColumnNotFound, name);
        return result;
    }
    pObject TableDesc::getObject(const std::vector<Row*>& rows, const std::string& tbl, const std::string& name) const {
        auto idx = getIndex(tbl, name);
        for (auto& row : rows)
            if (idx < row->objs.size())
                return row->objs[idx];
            else
                idx -= row->objs.size();
        //TODO
        throw "Not Imp";
    }
    pObject TableDesc::getObject(const Row& row, const std::string& tbl, const std::string& name) const {
        auto idx = getIndex(tbl, name);
        return row.objs[idx];
    }
    pObject TableDesc::getPrimary(const Row& row) const {
        return row.objs[primaryIndex];
    }
    std::vector<pObject> Table::getVec(const std::string& tbl, const std::string& name) const {
        std::size_t index = desc.getIndex(tbl, name);
        std::vector<pObject> ret;
        for (auto row : rows)
            ret.push_back(row.objs[index]);
        return std::move(ret);
    }
}
