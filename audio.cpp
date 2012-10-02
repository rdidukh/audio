l#include <iostream>
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

public:
	
	WaveFileHeader()
	{
		this->id << "RIFF";
		this->size = 100; //
		this->type = "WAVE";
	}

	virtual void serialize(std::ofstream& os)
	{
		os << this->id << this->size << this->type;

		for(std::vector<WaveFileChunk>::iterator i = chunks.begin(); i != chunks.end(); ++i)
			i->serialize(os);

	}

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
public:
	virtual void serialize(std::ofstream& os)
	{
		os << this->id << this->size << this->compression << this->channels << this->rate << this->bpsec << this->align << this->sample;
	}

	WaveFileFormatChunk(uint16_t compression, uint16_t channels, uint32_t rate, uint32_t bpsec, uint16_t align, uint16_t sample)
	{
		this->id = "fmt ";
		this->size = 16;
		this->compression = compression;
		this->channels = channels;
		this->rate = rate;
		this->bpsec = bpsec;
		this->align = align;
		this->sample = sample;
	}

};

class WaveFileDataChunk: public WaveFileChunk
{
	// void * data;
	

};

class WaveForm
{
	double S(unsigned int t);
	public:
	double operator()();
//	WaveForm operator+(const WaveForm& wf);
};

template<typename SampleType>
SampleType modulate(double val, double max, int bits)
{
	SampleType sample = 0;

	return sample;
}

/*
class WaveFormMixer: public WaveForm
{
	std::vector<std::pair<WaveForm&, double>> wfs;

	public:
	void add(WaveForm& wf, double amp)
	{
		wfs.push_back(std::make_pair<WaveForm&, double>(wf, amp));
	}

	WaveForm mix();

	void clear()
	{
		wfs.clear();
	}

};

*/

int main(int argc, char** argv)
{
	std::ofstream outfile;
		
	outfile.open(argv[1], ios::out | ios::binary | ios::trunc | ios::app);	

	if(outfile.fail())
	{
		std::cout << "Can not open " << argv[1] << " for writing." << std::endl;
		exit(1);
	}

	

	outfile.close();

	return 0;

}


