#include <string>

namespace Utils {
    typedef unsigned char byte;
    typedef unsigned short word;


    inline byte readByte(const char*& buf) {
        byte ret = *(byte*)buf;
        buf += 1;
        return ret;
    }
    inline byte readByte(char*& buf) {
        byte ret = *(byte*)buf;
        buf += 1;
        return ret;
    }
    inline void jumpByte(const char*& buf) {
        buf += 1;
    }
    inline void jumpByte(char*& buf) {
        buf += 1;
    }
    inline void writeByte(char*& buf, byte v) {
        *(byte*)buf = v;
        buf += 1;
    }


    inline word readWord(const char*& buf) {
        word ret = *(word*)buf;
        buf += 2;
        return ret;
    }
    inline word readWord(char*& buf) {
        word ret = *(word*)buf;
        buf += 2;
        return ret;
    }
    inline void jumpWord(const char*& buf) {
        buf += 2;
    }
    inline void jumpWord(char*& buf) {
        buf += 2;
    }
    inline void writeWord(char*& buf, word v) {
        *(word*)buf = v;
        buf += 2;
    }


    inline int readInt(const char*& buf) {
        int ret = *(int*)buf;
        buf += 4;
        return ret;
    }
    inline int readInt(char*& buf) {
        int ret = *(int*)buf;
        buf += 4;
        return ret;
    }
    inline void jumpInt(const char*& buf) {
        buf += 4;
    }
    inline void jumpInt(char*& buf) {
        buf += 4;
    }
    inline void writeInt(char*& buf, int v) {
        *(int*)buf = v;
        buf += 4;
    }


    inline std::string readString(const char*& buf) {
        word len = readWord(buf);
        std::string ret(buf, buf + len);
        buf += len;
        return ret;
    }
    inline std::string readString(char*& buf) {
        word len = readWord(buf);
        std::string ret(buf, buf + len);
        buf += len;
        return ret;
    }
    inline void jumpString(const char*& buf) {
        word len = readWord(buf);
        buf += len;
    }
    inline void jumpString(char*& buf) {
        word len = readWord(buf);
        buf += len;
    }
    inline void writeString(char*& buf, const std::string& v) {
        writeWord(buf, v.size());
        for (auto c : v)
            writeByte(buf, c);
    }
}
