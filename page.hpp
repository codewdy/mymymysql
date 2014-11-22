#ifndef WDY_8320182910_PAGE
#define WDY_8320182910_PAGE
#include "file.hpp"

namespace PageDB {
    const int PAGE_SIZE = 8192;
    struct PageHeader {
    };

    struct Page {
        PageHeader header;
        char buf[PAGE_SIZE];
    };

    struct PageDesc {
        File* file;
        int page_id;
        Page* page;
        int ref;
        bool dirty;
        PageDesc(File* _file, int _page_id)
            : file(_file), page_id(_page_id),
            page(nullptr), ref(0), dirty(false)
        {}
        void incRef() {
            ref++;
            if (page == nullptr) {
                page = file->loadPage(page_id);
            }
        }
        void decRef() {
            ref--;
        }
        void writeAndDecRef() {
            dirty = true;
            decRef();
        }
        void WriteBack() {
            if (dirty) {
                file->writePage(page_id, page);
                dirty = false;
            }
        }
        void Release() {
            delete page;
            page = nullptr;
        }
    };

    struct PageSession {
        PageDesc* desc;
        PageSession(PageDesc* _desc) : desc(_desc) {
            desc->incRef();
        }
        PageSession(PageSession&& pgs) : desc(pgs.desc) {pgs.desc = nullptr;}
        ~PageSession() {
            if (desc)
                desc->decRef();
        }
        Page& page() {
            return *(desc->page);
        }
        
    private:
        PageSession(const PageSession&);
        PageSession& operator=(const PageSession&);
    };
}
#endif
