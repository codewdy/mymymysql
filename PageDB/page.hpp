#ifndef WDY_8320182910_PAGE
#define WDY_8320182910_PAGE
#include "file.hpp"
#include <mutex>

namespace PageDB {
    struct Page {
        char buf[PAGE_SIZE];
    };

    struct PageDesc {
        File* file;
        int page_id;
        Page* page;
        std::mutex ref_mutex;
        int ref;
        bool dirty;
        bool used;
        PageDesc(File* _file, int _page_id)
            : file(_file), page_id(_page_id),
            page(nullptr), ref_mutex(), ref(0), dirty(false), used(false)
        {}
        void incRef() {
            used = true;
            ref_mutex.lock();
            ref++;
            if (page == nullptr) {
                page = file->loadPage(page_id);
            }
            ref_mutex.unlock();
        }
        void decRef() {
            ref_mutex.lock();
            ref--;
            ref_mutex.unlock();
        }
        void write() {
            dirty = true;
        }
        void WriteBack() {
            if (dirty) {
                dirty = false;
                file->writePage(page_id, page);
            }
        }
        void Release(bool force = true) {
            if (force) {
                ref_mutex.lock();
            } else {
                if (!ref_mutex.try_lock()) {
                    return;
                }
            }
            if (force || ref == 0) {
                WriteBack();
                delete page;
                page = nullptr;
            }
            ref_mutex.unlock();
        }
        ~PageDesc() {
            Release();
        }
        void Remove() {
            ref_mutex.lock();
            dirty = false;
            ref = 0;
            delete page;
            page = nullptr;
            ref_mutex.unlock();
        }
    };

    struct PageSession {
        PageDesc* desc;
        PageSession(PageDesc* _desc = nullptr) : desc(_desc) {
            incRef();
        }
        PageSession(PageSession&& pgs) : desc(pgs.desc) {pgs.desc = nullptr;}
        PageSession(const PageSession& pgs) : desc(pgs.desc) {incRef();}
        PageSession& operator=(PageSession&& pgs) {
            decRef();
            desc = pgs.desc;
            pgs.desc = nullptr;
            return * this;
        }
        PageSession& operator=(const PageSession& pgs) {
            decRef();
            desc = pgs.desc;
            incRef();
            return * this;
        }
        void decRef() {
            if (desc)
                desc->decRef();
        }
        void incRef() {
            if (desc)
                desc->incRef();
        }
        ~PageSession() {
            decRef();
            desc = nullptr;
        }
        const Page& page() {
            return *(desc->page);
        }
        const char* buf() {
            return page().buf;
        }
    };

    struct PageWriteSession {
        PageDesc* desc;
        PageWriteSession(PageDesc* _desc = nullptr) : desc(_desc) {
            incRef();
        }
        PageWriteSession(PageWriteSession&& pgs) : desc(pgs.desc) {pgs.desc = nullptr;}
        PageWriteSession(const PageWriteSession& pgs) : desc(pgs.desc) {incRef();}
        PageWriteSession& operator=(PageWriteSession&& pgs) {
            decRef();
            desc = pgs.desc;
            pgs.desc = nullptr;
            return * this;
        }
        PageWriteSession& operator=(const PageWriteSession& pgs) {
            decRef();
            desc = pgs.desc;
            incRef();
            return * this;
        }
        void flush() {
            if (desc) {
                desc->write();
            }
        }
        void decRef() {
            if (desc) {
                desc->write();
                desc->decRef();
            }
        }
        void incRef() {
            if (desc)
                desc->incRef();
        }
        void Release() {
            decRef();
            desc = nullptr;
        }
        ~PageWriteSession() {
            Release();
        }
        Page& page() {
            return *(desc->page);
        }
        char* buf() {
            return page().buf;
        }
    };
}
#endif
