#include "schedule.hpp"
#include <thread>
#include <chrono>

namespace PageDB {
    void Scheduler::ScheduleLoop() {
        while (running) {
            std::this_thread::sleep_for(std::chrono::seconds(SCHEDULE_LOOP_DELAY));
            Schedule();
        }
    }
    void Scheduler::StartSchedule() {
        if (!running) {
            running = true;
            ScheduleThread = std::thread(&Scheduler::ScheduleLoop, this);
        }
    }
    void Scheduler::StopSchedule() {
        if (running) {
            running = false;
            ScheduleThread.join();
        }
    }
    File* Scheduler::OpenFile(const std::string& fn) {
        File*& file = fileIndex[fn];
        if (file == nullptr) {
            file = new File(fn);
        }
        return file;
    }
    PageDesc* Scheduler::GetPage(File* file, int page_id) {
        PageDesc*& desc = pageIndex[std::make_pair(file, page_id)];
        if (desc == nullptr) {
            desc = new PageDesc(file, page_id);
        }
        return desc;
    }
    Scheduler::~Scheduler() {}
}
