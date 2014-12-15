#include "tblExpr.hpp"
#include "Exception/Exception.hpp"

namespace TypeDB {
    TblExpr::~TblExpr() {}
    std::vector<pObject> ReadTblExpr::Calc(const Table& table) {
        return table.getVec(tbl, name);
    }
    std::vector<pObject> UnaryTblExpr::Calc(const Table& table) {
        std::vector<pObject> org = l->Calc(table);
        auto beg = org.begin(), end = org.end();
        pObject retX = *beg++;
        switch (op) {
        case Sum:
        {
            for (auto iter = beg; iter != end; iter++)
                retX = retX->op_add(*iter);
            break;
        }
        case Avg:
        {
            for (auto iter = beg; iter != end; iter++)
                retX = retX->op_add(*iter);
            Object* raw = retX;
            Int* i = dynamic_cast<Int*>(raw);
            int r = i->raw;
            retX = new Double((double)r / org.size());
            break;
        }
        case Min:
        {
            for (auto iter = beg; iter != end; iter++)
                retX = retX->op_lt(*iter) ? retX : *iter;
            break;
        }
        case Max:
            for (auto iter = beg; iter != end; iter++)
                retX = retX->op_gt(*iter) ? retX : *iter;
            break;
        }
        return std::vector<pObject>(1, std::move(retX));
    }
    UnaryTblExpr::~UnaryTblExpr() {delete l;}
}
