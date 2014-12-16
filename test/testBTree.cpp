#include "BTree/BTree.hpp"
#include "PageDB/lru_scheduler.hpp"
#include <iostream>

int main() {
    PageDB::Scheduler* pgdb = new PageDB::LRU_Scheduler;
    pgdb->StartSchedule();
    std::string idx;
    std::getline(std::cin, idx);
    BTree::BTree bt(pgdb, "testBTree.txt");
    auto X = bt.find(idx);
    auto Y = X.second;
    Y.Page += 2;
    Y.Offset += 1;
    if (!X.first) {
        Y.Page = 0;
        Y.Offset = 0;
        bt.set(idx, Y, true);
    }
    std::cout << Y.Page << " " << Y.Offset << std::endl;
    pgdb->StopSchedule();
    delete pgdb;
}
