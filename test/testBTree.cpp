#include "BTree/BTree.hpp"
#include "PageDB/lru_scheduler.hpp"
#include <iostream>
#include <algorithm>
#include <ctime>
#include <cstdio>
#include <set>

void test(PageDB::Scheduler* pgdb) {
    int ALL = 10000, DEL = 5000;
    std::srand((unsigned int)time(NULL));
    std::remove("testBTree.txt");
    BTree::BTree bt(pgdb, "testBTree.txt");
    for (int t = 0; t < 10; t++) {
        std::vector<int> ava, removed;
        bt.set("test", BTree::Value(0, 0), true);
        bt.remove("test");
        auto X = bt.find("test");
        if (X.first)
            std::cout << "ERROR" << std::endl;
        for (int i = 0; i < ALL; i++) {
            bt.set(std::to_string(i), BTree::Value(i * 2, i), true);
            ava.push_back(i);
        }
        for (int i = 0; i < DEL; i++) {
            int pos = rand() % ava.size();
            bt.remove(std::to_string(ava[pos]));
            removed.push_back(ava[pos]);
            ava[pos] = ava.back();
            ava.pop_back();
        }
        for (int i : ava) {
            //std::cout << "tesing" << i << std::endl;
            auto V = bt.find(std::to_string(i));
            auto X = V.second;
            if (!V.first)
                std::cout << "ERROR1!" << i << std::endl;
            if (X.Page != i * 2)
                std::cout << "ERROR2!" << i << std::endl;
            if (X.Offset != i)
                std::cout << "ERROR3!" << i << std::endl;
        }
        for (int i : removed) {
            //std::cout << "tesing" << i << std::endl;
            auto V = bt.find(std::to_string(i));
            if (V.first)
                std::cout << "ERROR4!" << i << std::endl;
        }
        for (int i = 0; i < ALL - DEL; i++) {
            int pos = rand() % ava.size();
            bt.remove(std::to_string(ava[pos]));
            removed.push_back(ava[pos]);
            ava[pos] = ava.back();
            ava.pop_back();
        }
    }
}

int main() {
    PageDB::Scheduler* pgdb = new PageDB::LRU_Scheduler;
    pgdb->StartSchedule();
    test(pgdb);
    pgdb->StopSchedule();
    delete pgdb;
}
