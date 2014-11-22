#ifndef WDY_1829373821_LRU_SCHEDULER
#define WDY_1829373821_LRU_SCHEDULER
#include "scheduler.hpp"
#include <map>

namespace PageDB {
    class LRU_Scheduler : public Scheduler {
        std::map<PageDesc*, int> lruIndex;
        virtual void Schedule();
    };
}

#endif
