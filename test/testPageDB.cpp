#include "PageDB/lru_scheduler.hpp"
#include "PageDB/iterator.hpp"
#include <iostream>

void test(PageDB::Scheduler* pgdb) {
    PageDB::Iterator X(pgdb, "testPageDB.txt");
    PageDB::ConstIterator Y(pgdb, "testPageDB.txt");
    int newPage = X.file->newPage();
    Y.Goto(newPage - 1, 0);
    int ans = Y.Get()[0];
    std::cout << newPage << std::endl;
    std::cout << ans << std::endl;
    X.Goto(newPage, 0);
    X.Get()[0] = ans + 1;
}

int main() {
    PageDB::LRU_Scheduler* pgdb = new PageDB::LRU_Scheduler;
    pgdb->StartSchedule();
    test(pgdb);
    pgdb->StopSchedule();
    delete pgdb;
}
