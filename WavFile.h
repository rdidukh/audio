#ifndef _WAV_FILE_H_
#define _WAV_FILE_H_

#include <string>
#include <cstring>
#include <fstream>
#include <iostream>
#include <cassert>
#include <cstdint>
#include <vector>

class WavFile
{
	bool failBit;
	std::string failStr;
	uint32_t chunkSize;

	uint16_t numChannels;
	uint32_t sampleRate;
	uint32_t byteRate;
	uint16_t blockAlign;
	uint16_t bitsPerSample;
	uint32_t dataSize;
	unsigned int numFrames;
	unsigned int index; // index of current frame
	unsigned int index1000; // 

	std::vector<uint8_t> data;

	template<class T> void read(std::istream & is, T & t);

	public:

	bool fail() const;

	const std::string & failString() const;

	WavFile(const std::string & fileName);
	
	unsigned long getFrames(char * buffer, unsigned long frames)
	{
		unsigned long bytesPerFrame = numChannels*bitsPerSample/8;

		if(index+frames > numFrames)
			frames = numFrames - index;
		
		memcpy(buffer, &data[index*bytesPerFrame], frames*bytesPerFrame);
	
		index += frames;
		index1000 += 1000*frames;				

		return frames;
	}

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

//		for(int i = 0; i < frames && index
		return res;
	}


	bool eof()
	{
		return index >= numFrames;
	}
};

#endif // _WAV_FILE_H_
