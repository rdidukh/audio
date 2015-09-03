#pragma once

#include "RingBuffer.h"
#include "AudioDevice.h"

class AudioInput
{
public:
    AudioInput(const AudioDevice & audioDevice, size_t bufferSize): mBuffer(bufferSize), mDevice(audioDevice), mInputDevice(nullptr)
    {

    }

private:
    RingBuffer mBuffer;
    const AudioDevice & mDevice;
    AudioDevice * mInputDevice;
};
