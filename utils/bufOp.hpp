#include <string>

namespace Utils {
    typedef unsigned char byte;
    typedef unsigned short word;


    byte readByte(const char*& buf) {
        byte ret = *(byte*)buf;
        buf += 1;
        return ret;
    }
    byte readByte(char*& buf) {
        byte ret = *(byte*)buf;
        buf += 1;
        return ret;
    }
    void jumpByte(const char*& buf) {
        buf += 1;
    }
    void jumpByte(char*& buf) {
        buf += 1;
    }
    void writeByte(char*& buf, byte v) {
        *(byte*)buf = v;
        buf += 1;
    }


    word readWord(const char*& buf) {
        word ret = *(word*)buf;
        buf += 2;
        return ret;
    }
    word readWord(char*& buf) {
        word ret = *(word*)buf;
        buf += 2;
        return ret;
    }
    void jumpWord(const char*& buf) {
        buf += 2;
    }
    void jumpWord(char*& buf) {
        buf += 2;
    }
    void writeWord(char*& buf, word v) {
        *(word*)buf = v;
        buf += 2;
    }


    int readInt(const char*& buf) {
        int ret = *(int*)buf;
        buf += 4;
        return ret;
    }
    int readInt(char*& buf) {
        int ret = *(int*)buf;
        buf += 4;
        return ret;
    }
    void jumpInt(const char*& buf) {
        buf += 4;
    }
    void jumpInt(char*& buf) {
        buf += 4;
    }
    void writeInt(char*& buf, int v) {
        *(int*)buf = v;
        buf += 4;
    }


    std::string readString(const char*& buf) {
        word len = readWord(buf);
        std::string ret(buf, buf + len);
        buf += len;
        return ret;
    }
    std::string readString(char*& buf) {
        word len = readWord(buf);
        std::string ret(buf, buf + len);
        buf += len;
        return ret;
    }
    void jumpString(const char*& buf) {
        word len = readWord(buf);
        buf += len;
    }
    void jumpString(char*& buf) {
        word len = readWord(buf);
        buf += len;
    }
    void writeString(char*& buf, const std::string& v) {
        writeWord(buf, v.size());
        for (auto c : v)
            writeByte(buf, c);
    }
}
