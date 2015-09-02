#pragma once

#include <cstddef>
class RingBuffer
{
public:
    explicit RingBuffer(size_t bufferSize);
    size_t size() const;
    size_t free() const;
    size_t avail() const;
    size_t write(const char * buffer, size_t count);
    size_t read(char * buffer, size_t count);

    template<class T> size_t write(const T & t)
    {
        if(free() < sizeof(t)) return 0;
        size_t ind = mRight < mSize ? mRight : mRight - mSize;
        if(sizeof(t) + ind <= mSize)
        {
            T * ptr = (T *)(&mBuffer[ind]);
            *ptr = t;
            mRight += sizeof(t);
            return sizeof(t);
        }
        else
        {
            write((const char *)&t, sizeof(t));
        }

        return sizeof(t);
    }

    template<class T> size_t read(T & t)
    {
        if(avail() < sizeof(t)) return 0;

        if(sizeof(t) + mLeft <= mSize)
        {
            T * ptr = reinterpret_cast<T *>(&mBuffer[mLeft]);
            t = *ptr;
            mLeft += sizeof(t);
            updateLeftRight();
        }
        else
        {
            read((char *)&t, sizeof(t));
        }

        return sizeof(t);
    }


private:

    void updateLeftRight();

    char * mBuffer;
    size_t mSize;
    size_t mLeft;
    size_t mRight;
};
