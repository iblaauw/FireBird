#pragma once

#include <cstring>

class IStringBuffer
{
public:
    virtual const char* GetData() = 0;
    virtual int Size() = 0;
};

class CharBuffer : public IStringBuffer
{
private:
    const char* data;
    int size;
    bool managed;
public:
    CharBuffer(const char* str, int length, bool copy) : size(length), managed(copy)
    {
        if (copy)
        {
            char* data_addr =  new char[length];
            memcpy(data_addr, str, length);
            data = data_addr;
        }
        else
        {
            data = str;
        }
    }

    ~CharBuffer()
    {
        if (managed)
            delete[] data;
    }

    const char* GetData() { return data; }
    int Size() { return size; }
};

