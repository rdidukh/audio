#include "RingBuffer.h"

#include <cstring>

RingBuffer::RingBuffer(size_t bufferSize): mBuffer(new char[bufferSize]), mSize(bufferSize), mLeft(0), mRight(0)
{

}

size_t RingBuffer::size() const
{
    return mSize;
}

size_t RingBuffer::free() const
{
    return mSize - (mRight - mLeft);
}

size_t RingBuffer::avail() const
{
    return mRight - mLeft;
}

size_t RingBuffer::write(const char * buffer, size_t count)
{
    if(count > free())
        count = free();

    size_t index = mRight;
    if(index >= mSize) index -= mSize;

    if(count + index <= mSize)
    {
        assert(index <= mSize);
        std::memcpy(&mBuffer[index], buffer, count);
    }
    else
    {
        assert(mSize > index);
        assert(count > (mSize-index));
        std::memcpy(&mBuffer[index], buffer, mSize-index);
        std::memcpy(&mBuffer[0], &buffer[mSize-index], count - (mSize-index));
    }

    mRight += count;

    selfTest();

    return count;
}

size_t RingBuffer::read(char * buffer, size_t count)
{
    if(count > avail())
        count = avail();

    if(mLeft + count <= mSize)
    {
        std::memcpy(buffer, &mBuffer[mLeft], count);
    }
    else
    {
        std::memcpy(buffer, &mBuffer[mLeft], mSize-mLeft);
        std::memcpy(&buffer[mSize-mLeft], &mBuffer[0], count-(mSize-mLeft));
    }

    mLeft += count;

    updateLeftRight();

    return count;
}

void RingBuffer::updateLeftRight()
{
    if(mLeft >= mSize)
    {
        mLeft -= mSize;
        mRight -= mSize;
    }

    if(mLeft == mRight)
    {
        mLeft = 0;
        mRight = 0;
    }
    selfTest();
}


