#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdint.h> // for uint32_t
#include <vector>

using namespace std;

class WaveFileChunk
{
	protected:
	std::string id;
	uint32_t size;
	public:
		virtual void serialize(std::ofstream& os)=0;
};

class WaveFileHeader: public WaveFileChunk
{
	std::string type;
	std::vector<WaveFileChunk *> chunks;

public:
	
	void add(WaveFileChunk* wfc)
	{
		this->chunks.push_back(wfc);
	}

	WaveFileHeader()
	{
		this->id = "RIFF";
		this->size = 100; // IMPLEMENT SIZE COUNT
		this->type = "WAVE";
	}

	virtual void serialize(std::ofstream& os)
	{
		os << this->id; //<< this->size << this->type;
		os.write((char *)&(this->size), sizeof(this->size));	// IMPLEMENT SIZE COUNT	
		os << this->type;

		for(std::vector<WaveFileChunk*>::iterator i = this->chunks.begin(); i != this->chunks.end(); ++i)
			(*i)->serialize(os);
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
		os << this->id;
		os.write((char *)&(this->size), sizeof(this->size));
		os.write((char *)&(this->compression), sizeof(this->compression));
		os.write((char *)&(this->channels), sizeof(this->channels));
		os.write((char *)&(this->rate), sizeof(this->rate));
		os.write((char *)&(this->bpsec), sizeof(this->bpsec));
		os.write((char *)&(this->align), sizeof(this->align));
		os.write((char *)&(this->sample), sizeof(this->sample));
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
	public:
	WaveFileDataChunk()
	{
		this->id = "data";
		this->size = 2049;	// IMPLEMENT SIZE COUNT
	}
	
	virtual void serialize(std::ofstream& os)
	{
		os << this->id;
		os.write((char *)&(this->size), sizeof(this->size));
		
		/* IMPLEMENT DATA SERIALIZATION!!! */

	}

};

class WaveForm
{
	double S(unsigned int t);
	public:
	double operator()();
//	WaveForm operator+(const WaveForm& wf);
};


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
		
	outfile.open(argv[1], ofstream::out | ofstream::binary | ofstream::trunc);// | ofstream::binary | ofstream::trunc | ofstream::app);	

	if(outfile.fail())
	{
		std::cout << "Can not open " << argv[1] << " for writing." << std::endl;
		exit(1);
	}

	WaveFileHeader wfh;
//WaveFileFormatChunk(uint16_t compression, uint16_t channels, uint32_t rate, uint32_t bpsec, uint16_t align, uint16_t sample)

	WaveFileFormatChunk fmt(1, 1, 44100, 0x123456, 4, 16);
	WaveFileDataChunk data;	

	wfh.add(&fmt);
	wfh.add(&data);

	wfh.serialize(outfile);

	outfile.close();

	return 0;
}

