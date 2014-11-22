#ifndef WDY_9282100293_SCHEDULER
#define WDY_9282100293_SCHEDULER
#include "file.hpp"
#include "page.hpp"
#include <map>
#include <string>
#include <thread>

namespace PageDB {
    //Sleep Seconds
    const int SCHEDULE_LOOP_DELAY = 100;
    class Scheduler {
        std::thread ScheduleThread;
        void ScheduleLoop();
        bool running;
    protected:
        std::map<std::string, File*> fileIndex;
        std::map<std::pair<File*, int>, PageDesc*> pageIndex;
        virtual void Schedule() = 0;
    public:
        void StartSchedule();
        void StopSchedule();
        bool Running() {
            return running;
        }
        File* OpenFile(const std::string& fn);
        PageDesc* GetPage(File* file, int page_id);
        virtual ~Scheduler();
    };
    class LRUScheduler : public Scheduler{
    };
}

#endif
