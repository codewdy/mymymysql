#ifndef WDY_0192310212_UTILS_WRITEFILE
#define WDY_0192310212_UTILS_WRITEFILE
#include <utility>
#include <memory>
#include "PageDB/scheduler.hpp"

namespace Utils {
    std::pair<unsigned short, unsigned short> writeFile(PageDB::Scheduler* pgdb, PageDB::File* file, const char* buf, int size) {
        if (file->eofOffset + size > PageDB::PAGE_SIZE) {
            file->eofPage = file->newPage();
            file->eofOffset = 0;
        }
        PageDB::PageWriteSession session = pgdb->GetWriteSession(file, file->eofPage);
        memcpy(session.buf() + file->eofOffset, buf, size);
        std::pair<unsigned short, unsigned short> ret;
        ret.first = file->eofPage;
        ret.second = file->eofOffset;
        file->eofOffset += size;
        file->writebackFileHeaderCore();
        return ret;
    }
}

#endif
