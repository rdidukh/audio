#ifndef _SOUND_STREAM_H_
#define _SOUND_STREAM_H_

#include <string>
#include <exception>
#include <vector>
#include <algorithm>
#include <cstring>

class SoundStreamException
{
    std::string messageVar;

    public:

    SoundStreamException(std::string message)
    {
        messageVar = message;
    }

    const std::string what() const
    {
        return messageVar;
    }
};

class SoundStream
{
    public:

    SoundStream():failBit(false), eofBit(false)
    {}

    virtual unsigned int channels() const noexcept
    {
        return numChannelsVar;
    }

    virtual unsigned int sampleRate() const noexcept
    {
        return sampleRateVar;
    }

    virtual unsigned int sampleSize() const noexcept
    {
        return sampleSizeVar;
    }

    virtual unsigned int frameSize() const  noexcept
    {
        return numChannelsVar * sampleSizeVar / 8;
    }

    unsigned int bufferSize() const
    {
        return bufferSizeVar;
    }

    virtual void setBufferSize(unsigned long newBufferSize)
    {
        bufferSizeVar = newBufferSize;
    }

//    virtual unsigned long readFrames(char * buffer) = 0;
    virtual unsigned long readFrames(char * buffer, unsigned long frames) = 0;

//    unsigned long writeFrames(const char * buffer) = 0;
    virtual unsigned long writeFrames(const char * buffer, unsigned long frames) = 0;

    bool fail() const noexcept
    {
        return failBit;
    }

    virtual bool eof() const noexcept
    {
        return eofBit;
    }

    const std::string & failString() const noexcept
    {
        return failStringVar;
    }

    void setChannels(unsigned int newChannels) throw(SoundStreamException)
    {
        numChannelsVar = newChannels;
    }

    void setSampleRate(unsigned int newSampleRate) throw(SoundStreamException)
    {
        sampleRateVar = newSampleRate;
    }

    void setSampleSize(unsigned int newSampleSize) throw(SoundStreamException)
    {
        sampleSizeVar = newSampleSize;
    }

protected:
    void setFailString(const std::string & newFailString) noexcept
    {
        failStringVar = newFailString;
        failBit = true;
    }

    void setEof(bool newEof) noexcept
    {
        eofBit = newEof;
    }

    void setFail(bool newFail) noexcept
    {
        failBit = newFail;
    }

    private:

    unsigned int numChannelsVar;
    unsigned int sampleRateVar;
    unsigned int sampleSizeVar; // in bits for one channel
    unsigned long bufferSizeVar; // in frames
    std::string failStringVar;
    bool failBit;
    bool eofBit;
};

class InputSoundStream: public SoundStream
{
    public:
    virtual unsigned long writeFrames(const char *buffer, unsigned long frames)
    {
        throw SoundStreamException("Can't read from OutputSoundStream.");
    }
};

class OutputSoundStream: public SoundStream
{
    public:
    virtual unsigned long readFrames(char *buffer, unsigned long frames)
    {
        throw SoundStreamException("Can't write to InputSoundStream.");
    }
};

class ConnectorSoundStream: public SoundStream
{
public:
    virtual void setBufferSize(unsigned long newBufferSize)
    {
        data.resize(newBufferSize * frameSize());
        SoundStream::setBufferSize(newBufferSize);
    }

    virtual unsigned long readFrames(char *buffer, unsigned long frames) throw(SoundStreamException)
    {
        if(frames < bufferSize())
            frames = bufferSize();
        std::memcpy(buffer, &data[0], frames*frameSize());
        return frames;
    }

    virtual unsigned long writeFrames(const char *buffer, unsigned long frames) throw(SoundStreamException)
    {
        if(frames < bufferSize())
            frames = bufferSize();
        std::memcpy(&data[0], buffer, frames*frameSize());
        return frames;
    }
private:

    std::vector<char> data;
};

#endif // _SOUND_STREAM_H_
