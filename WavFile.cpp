#include <string>
#include <fstream>
#include <iostream>
#include <cassert>
#include <cstdint>
#include <vector>

#include "WavFile.h"

template<class T> void WavFile::read(std::istream & is, T & t)
{
	is.read(reinterpret_cast<char *>(&t), sizeof(T));
	if(is.eof()) throw std::string("EOF");
	if(is.fail()) throw std::string("FAIL");
	if(is.bad()) throw std::string("BAD");
}

bool WavFile::fail() const
{
	return failBit;
}

const std::string & WavFile::failString() const
{
	return failStr;
}

WavFile::WavFile(const std::string & fileName): failBit(false), index(0)
{
	try
	{
		std::ifstream file(fileName);
	
		if(file.fail())	throw std::string("File not found.");		

		char chunkId[4];

		read(file, chunkId);

		if(std::string(chunkId, 4) != "RIFF")
			throw std::string("Not a wave file(1)");			

		read(file, chunkSize);

		read(file, chunkId);

		if(std::string(chunkId, 4) != "WAVE")
			throw std::string("Not a wave file(2)");

		read(file, chunkId);

		if(std::string(chunkId, 4) != "fmt ")
			throw std::string("Not a wave file(3)");

		uint32_t subchunkSize;

		read(file, subchunkSize);
			
		// if(subchunkSize != 16)
		//	throw std::string("Not supported fmt subchunk size: ")+std::to_string(subchunkSize);

		uint16_t audioFormat;
		
		read(file, audioFormat);

		if(audioFormat != 1)
			throw std::string("Not supported audio format: ")+std::to_string(audioFormat);

		read(file, numChannels);

		if(numChannels != 1 && numChannels != 2)
			throw std::string("Not supported number of channels: ")+std::to_string(numChannels);

		read(file, sampleRate);

		read(file, byteRate);

		read(file, blockAlign);

		read(file, bitsPerSample);

		if(subchunkSize > 16)
			file.ignore(subchunkSize-16);

		read(file, chunkId);

		if(std::string(chunkId, 4) != "data")
			throw std::string("Unexpected chunk id: ") + std::string(chunkId, 4);

		read(file, dataSize);

		data.resize(dataSize);

		file.read((char *)&data[0], dataSize);

		if(file.fail())
			throw std::string("Not a wave file(5)");

		numFrames = dataSize/(numChannels*bitsPerSample/8);

		file.close();
	}
	catch(const std::string & str)
	{
		failBit = true;
		failStr = str;
	}
}

