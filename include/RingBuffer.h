#pragma once

#include <cstddef>
#include <cassert>
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
        assert(ind >= 0 && ind < mSize);
        if(sizeof(t) + ind <= mSize)
        {
            T * ptr = (T *)(&mBuffer[ind]);
            *ptr = t;
            mRight += sizeof(t);
        }
        else
        {
            write((const char *)&t, sizeof(t));
        }

        selfTest();

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

        selfTest();

        return sizeof(t);
    }


private:

    void updateLeftRight();

    inline void selfTest() const
    {
        assert(mLeft >= 0 && mRight >= 0);
        assert(mRight > mLeft || (mRight == 0 && mLeft == 0));
        assert(mLeft < mSize);
        assert(mRight < mSize+mSize);
        assert(mRight - mLeft <= mSize);
    }

    char * mBuffer;
    size_t mSize;
    size_t mLeft;
    size_t mRight;
};
