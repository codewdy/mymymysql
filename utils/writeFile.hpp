#ifndef WDY_0192310212_UTILS_WRITEFILE
#define WDY_0192310212_UTILS_WRITEFILE
#include <utility>
#include <memory>
#include "PageDB/scheduler.hpp"

namespace Utils {
    inline PageDB::Location writeFile(PageDB::Scheduler* pgdb, PageDB::File* file, const char* buf, int size) {
        if (file->eof.Offset + size > PageDB::PAGE_SIZE) {
            file->eof.Page = file->newPage();
            file->eof.Offset = 0;
        }
        PageDB::PageWriteSession session = pgdb->GetWriteSession(file, file->eof.Page);
        memcpy(session.buf() + file->eof.Offset, buf, size);
        PageDB::Location ret = file->eof;
        file->eof.Offset += size;
        file->writebackFileHeaderCore();
        return ret;
    }
}

#endif
