#include <iostream>
#include <ifstream>
#include <cstdlib>
#include <vector>

using namespace std;

class WaveFileChunk
{
	std::string id;
	uint32_t size;
	public:
		virtual void serialize(std::ofstream& os)=0;
};

class WaveFileHeader: public WaveFileChunk
{
	std::string type;
	std::vector<WaveFileChunk> chunks;
};

class WaveFileFormatChunk: public WaveFileChunk
{
	uint16_t compression;
	uint16_t channels;
	uint32_t rate;
	uint32_t bpsec;
	uint16_t align;
	uint16_t sample;
//	uint16_t extra_size;
};

class WaveFileDataChunk: public WaveFileChunk
{
	// void * data;
	

};

int main(int argc, char** argv)
{
	
	std::ofstream outfile;
	
	
	
}