#ifndef WDY_1920382934_FILE
#define WDY_1920382934_FILE
#include <cstdio>
#include <string>

namespace PageDB {
    struct Page;
    struct File {
        FILE* raw;
        File(const std::string&);
        Page* loadPage(int page_id);
        void writePage(int page_id, Page* pg);
    };
}
#endif
