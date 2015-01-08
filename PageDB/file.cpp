#include "file.hpp"
#include "page.hpp"
#include <cassert>

namespace PageDB {
    static char zeroPage[PAGE_SIZE] = {0};
    static inline void writeZeroPage(std::ostream& out, int paddr) {
        out.seekp(paddr * PAGE_SIZE);
        out.write(zeroPage, PAGE_SIZE);
    }
    int File::pageOffset(int vaddr) {
        auto iter = pageMap.find(vaddr);
        if (iter == pageMap.end()) {
            throw "Page Not Found";
        }
        //first page to save the file informations
        return (iter->second + 1) * PAGE_SIZE;
    }
    int File::newPage() {
        if (pageMap.size() >= MaxPage) {
            //TODO
            throw "Page Overflow";
        }
        std::unordered_set<int> Vs;
        for (auto& item : pageMap) {
            Vs.insert(item.second);
        }
        int vaddr = -1, paddr = -1;
        for (int i = 0; i < MaxPage + 10; i++)
            if (pageMap.find(i) == pageMap.end()) {
                vaddr = i;
                break;
            }
        for (int i = 0; i < MaxPage + 10; i++)
            if (Vs.find(i) == Vs.end()) {
                paddr = i;
                break;
            }
        pageMap[vaddr] = paddr;
        raw_mutex.lock();
        writeZeroPage(raw, paddr + 1);
        raw_mutex.unlock();
        writebackFileHeader();
        return vaddr;
    }
    void File::removePage(int pageid) {
        std::size_t count = pageMap.erase(pageid);
        if (count == 0) {
            //TODO
            throw "Page Not Found";
        }
        writebackFileHeader();
    }
    void File::initFile() {
        entryPageID = 0;
        eof.Page = 0;
        eof.Offset = 0;
        pageMap.clear();
        pageMap[0] = 0;
        raw_mutex.lock();
        writeZeroPage(raw, 0);
        writeZeroPage(raw, 1);
        raw_mutex.unlock();
        writebackFileHeader();
    }
    void File::writebackFileHeaderCore(bool lock) {
        assert(raw);
        if (lock)
            raw_mutex.lock();
        raw.seekp(0);
        raw.write((const char*)&MagicNumber, 4);
        raw.write((const char*)&entryPageID, 2);
        raw.write((const char*)&eof.Page, 2);
        raw.write((const char*)&eof.Offset, 2);
        if (lock)
            raw_mutex.unlock();
    }
    void File::writebackFileHeader() {
        raw_mutex.lock();
        writebackFileHeaderCore(false);
        unsigned short tmp;
        tmp = pageMap.size();
        raw.write((const char*)&tmp, 2);
        for (auto& item : pageMap) {
            raw.write((const char*)&item.first, 2);
            raw.write((const char*)&item.second, 2);
        }
        raw_mutex.unlock();
    }
    void File::readFile() {
        raw_mutex.lock();
        raw.seekg(0);
        int magic;
        unsigned short tmp = 0;
        raw.read((char*)&magic, 4);
        if (magic != MagicNumber) {
            raw_mutex.unlock();
            initFile();
            return;
        }
        raw.read((char*)&tmp, 2);
        entryPageID = tmp;
        raw.read((char*)&tmp, 2);
        eof.Page = tmp;
        raw.read((char*)&tmp, 2);
        eof.Offset = tmp;
        raw.read((char*)&tmp, 2);
        int size = tmp;
        for (int i = 0; i < size; i++) {
            unsigned short vaddr, paddr;
            raw.read((char*)&vaddr, 2);
            raw.read((char*)&paddr, 2);
            pageMap[vaddr] = paddr;
        }
        raw_mutex.unlock();
    }
    File::File(const std::string& fn) : raw(fn, std::ios::in | std::ios::out | std::ios::binary) {
        if (!raw) {
            raw_mutex.lock();
            raw.open(fn, std::ios::out | std::ios::binary | std::ios::trunc);
            raw.close();
            raw.open(fn, std::ios::in | std::ios::out | std::ios::binary);
            raw_mutex.unlock();
            initFile();
        } else {
            readFile();
        }
    };
    Page* File::loadPage(int page_id) {
        assert(raw);
        int offset = pageOffset(page_id);
        Page* ret = new Page;
        raw_mutex.lock();
        raw.seekg(offset);
        raw.read(ret->buf, PAGE_SIZE);
        raw_mutex.unlock();
        return ret;
    }
    void File::writePage(int page_id, Page* pg) {
        assert(raw);
        int offset = pageOffset(page_id);
        raw_mutex.lock();
        raw.seekp(offset);
        raw.write(pg->buf, PAGE_SIZE);
        raw_mutex.unlock();
    }
}
