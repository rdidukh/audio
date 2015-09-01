#ifndef _WAV_FILE_H_
#define _WAV_FILE_H_

#include <string>
#include <cstring>
#include <fstream>
#include <iostream>
#include <cassert>
#include <cstdint>
#include <vector>
#include <cstdint>

#include "SoundStream.h"

class WavFileInputSoundStream: public InputSoundStream
{
	uint32_t chunkSize;
	uint32_t byteRate;
	uint16_t blockAlign;
	uint32_t dataSize;
	unsigned int numFrames;
	unsigned int index; // index of current frame
    //unsigned int index1000; //

	std::vector<uint8_t> data;

	template<class T> void read(std::istream & is, T & t);

	public:

    WavFileInputSoundStream(const std::string & fileName);
	
    virtual unsigned long readFrames(char * buffer, unsigned long frames)
	{
        unsigned long bytesPerFrame = this->frameSize();

		if(index+frames > numFrames)
			frames = numFrames - index;
		
		memcpy(buffer, &data[index*bytesPerFrame], frames*bytesPerFrame);
	
		index += frames;
    //	index1000 += 1000*frames;

		return frames;
	}

    /*
	unsigned long getFrames(char * buffer, unsigned long frames, int speed)
	{
		unsigned long bytesPerFrame = numChannels*bitsPerSample/8;
		unsigned long res = 0;
		for(unsigned long i = 0; i < frames && index < numFrames; i++)
		{
			memcpy(buffer, &data[index*bytesPerFrame], bytesPerFrame);
			buffer += bytesPerFrame;
			index1000 += speed; 
			index = index1000 / 1000;
			res++;
		}

		return res;
	}
    */

    virtual bool eof() const noexcept
    {
        return index >= numFrames;
    }
};

#endif // _WAV_FILE_H_
