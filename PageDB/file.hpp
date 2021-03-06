#ifndef WDY_1920382934_FILE
#define WDY_1920382934_FILE
#include <fstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <mutex>

namespace PageDB {
    struct Location {
        unsigned short Page, Offset;
        Location() {}
        Location(unsigned short _Page, unsigned short _Offset)
            : Page(_Page), Offset(_Offset) {}
    };
    inline bool operator==(Location lhs, Location rhs) {
        return lhs.Page == rhs.Page && lhs.Offset == rhs.Offset;
    }
    inline bool operator!=(Location lhs, Location rhs) {
        return !(lhs == rhs);
    }
    struct Page;
    const int PAGE_SIZE = 8192;
    const int MaxPage = 2045;
    const int MagicNumber = 0x19941102;
    const int MagicNumberOffset = 0;
    const int EntryPageOffset = MagicNumberOffset + 4;
    const int EofPageOffset = EntryPageOffset + 2;
    const int EofOffOffset = EofPageOffset + 2;
    const int PageCountOffset = EofOffOffset + 2;
    const int PageMapOffset = PageCountOffset + 2;
    struct File {
        std::fstream raw;
        std::mutex raw_mutex;
        int entryPageID;
        Location eof;
        std::unordered_map<int, int> pageMap;
        int pageOffset(int vaddr);
        int newPage();
        void removePage(int pageid);
        void writebackFileHeaderCore(bool lock = true);
        void writebackFileHeader();
        File(const std::string&);
        Page* loadPage(int page_id);
        void writePage(int page_id, Page* pg);
        ~File() {
            raw.close();
        }
        void initFile();
        void readFile();
    };
}
#endif
