#include "scheduler.hpp"
#include <thread>
#include <chrono>

namespace PageDB {
    void Scheduler::ScheduleLoop() {
        while (running) {
            std::this_thread::sleep_for(SCHEDULE_LOOP_DELAY);
            Schedule();
        }
    }
    void Scheduler::StartSchedule() {
        if (!running) {
            running = true;
            ScheduleThread = std::thread([=](){this->ScheduleLoop();});
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
    File* Scheduler::InitFile(const std::string& fn) {
        File*& file = fileIndex[fn];
        if (file == nullptr) {
            file = new File(fn);
        } else {
            auto beg = pageIndex.lower_bound(std::make_pair(file, 0));
            auto end = pageIndex.upper_bound(std::make_pair(file, 0x3FFFFFFF));
            for (auto iter = beg; iter != end; iter++)
                iter->second->Useless();
        }
        file->initFile();
        return file;
    }
    PageDesc* Scheduler::GetPage(File* file, int page_id) {
        if (page_id < 0)
            return nullptr;
        PageDesc*& desc = pageIndex[std::make_pair(file, page_id)];
        if (desc == nullptr) {
            desc = new PageDesc(file, page_id);
        }
        return desc;
    }
    void Scheduler::RemovePage(File* file, int page_id) {
        PageDesc*& desc = pageIndex[std::make_pair(file, page_id)];
        if (desc) {
            desc->Remove();
        }
        file->removePage(page_id);
    }
    PageSession Scheduler::GetSession(File* file, int page_id) {
        return PageSession(GetPage(file, page_id));
    }
    PageWriteSession Scheduler::GetWriteSession(File* file, int page_id) {
        return PageWriteSession(GetPage(file, page_id));
    }
    Scheduler::~Scheduler() {
        StopSchedule();
        for (auto& item : pageIndex)
            delete item.second;
        for (auto& item : fileIndex)
            delete item.second;
    }
}
