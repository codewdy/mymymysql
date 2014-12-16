#include "BTree/BTree.hpp"
#include "PageDB/lru_scheduler.hpp"
#include <iostream>

int main() {
    PageDB::Scheduler* pgdb = new PageDB::LRU_Scheduler;
    pgdb->StartSchedule();
    BTree::BTree bt(pgdb, "testBTree.txt");
    bt.set("test", BTree::Value(0, 0), true);
    bt.remove("test");
    auto X = bt.find("test");
    if (X.first)
        std::cout << "ERROR" << std::endl;
    for (int i = 0; i < 10000; i++)
        bt.set(std::to_string(i), BTree::Value(i * 2, i), true);
    for (int i = 0; i < 10000; i++) {
        std::cout << "tesing" << i << std::endl;
        auto V = bt.find(std::to_string(i));
        auto X = V.second;
        if (!V.first)
            std::cout << "ERROR!" << std::endl;
        if (X.Page != i * 2)
            std::cout << "ERROR!" << std::endl;
        if (X.Offset != i)
            std::cout << "ERROR!" << std::endl;
    }
    pgdb->StopSchedule();
    delete pgdb;
}
