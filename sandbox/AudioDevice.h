#ifndef _AUDIO_DEVICE_H_
#define _AUDIO_DEVICE_H_

#include "SoundStream.h"

#include <vector>
#include <cassert>

class AudioDevice
{
    public:
    virtual void process() = 0;
};

class SimpleAudioDevice: public AudioDevice
{
    public:

    SimpleAudioDevice(InputSoundStream & inputStream, OutputSoundStream & outputStream): input(&inputStream), output(&outputStream)
    {
        assert(input->channels() == output->channels());
        assert(input->frameSize() == output->frameSize());
        assert(input->sampleSize() == output->sampleSize());
        bufferSize = 4096;
        buffer.resize(bufferSize);
    }

    virtual void process()
    {
        unsigned long frames = bufferSize/input->frameSize();
        unsigned long readFrames = input->readFrames(&buffer[0], frames);
        output->writeFrames(&buffer[0], readFrames);
    }

    bool end()
    {
        return input->eof();
    }

    protected:

    private:
    InputSoundStream * input;
    OutputSoundStream * output;
    unsigned long bufferSize;
    std::vector<char> buffer;
};

#endif // _AUDIO_DEVICE_H_
