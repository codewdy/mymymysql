#include "context/context.hpp"
#include <iostream>
#include <algorithm>
#include <ctime>
#include <cstdio>
#include <set>

void test(PageDB::Scheduler* pgdb) {
    std::srand((unsigned int)time(NULL));
    std::remove("test.db");
    std::remove("test:wdy.db");
    std::remove("test:wdy.idx");
    Context::Context ctx(pgdb);
    TypeDB::TableDesc desc;
    TypeDB::ColDesc col_desc1, col_desc2, col_desc3;
    col_desc1.type = TypeDB::intType;
    col_desc2.type = TypeDB::stringType;
    col_desc3.type = TypeDB::stringType;
    col_desc1.name = "W";
    col_desc2.name = "D";
    col_desc3.name = "Y";
    desc.descs.push_back(col_desc1);
    desc.descs.push_back(col_desc2);
    desc.descs.push_back(col_desc3);
    ctx.InitTable("wdy", desc);
}

int main() {
    PageDB::Scheduler* pgdb = new PageDB::LRU_Scheduler;
    pgdb->StartSchedule();
    test(pgdb);
    pgdb->StopSchedule();
    delete pgdb;
}
