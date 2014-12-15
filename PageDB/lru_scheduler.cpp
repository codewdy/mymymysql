#include "lru_scheduler.hpp"

namespace PageDB {
    void LRU_Scheduler::Schedule() {
        for (auto iter = pageIndex.begin(); iter != pageIndex.end(); iter++) {
            PageDesc* desc = iter->second;
            desc->WriteBack();
            int& count = lruIndex[desc];
            if (desc->ref) {
                count++;
            } else {
                if (count) {
                    count--;
                } else {
                    desc->Release();
                }
            }
        }
    }
}
