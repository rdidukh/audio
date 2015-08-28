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
	unsigned int index;

	std::vector<uint8_t> data;

	template<class T> void read(std::istream & is, T & t);

	public:

	bool fail() const;

	const std::string & failString() const;

	WavFile(const std::string & fileName);
	
	unsigned long getFrames(char * buffer, unsigned long frames)
	{
		unsigned long bytesPerFrame = numChannels*bitsPerSample/8;
		unsigned long bytesToCopy = frames * bytesPerFrame;

		if(index + bytesToCopy > dataSize)
			bytesToCopy = dataSize-index;

		memcpy(buffer, &data[index], bytesToCopy);
	
		index += bytesToCopy;
				
		return bytesToCopy/bytesPerFrame;
	}

	bool eof()
	{
		return index == dataSize;
	}
};

#endif // _WAV_FILE_H_
