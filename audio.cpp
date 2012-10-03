#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <stdint.h> // for uint32_t
#include <vector>

using namespace std;

static const double PI = 3.14159265;

class WaveForm
{
	virtual double S(double x)=0;
	public:
	double operator()(double x)
	{
		return S(x);
	}
	
	uint16_t operator()(unsigned int t, unsigned int rate, double max, int bits)
	{
		double t_i = (double)t/rate;
		double s_i = (1<<(bits-1))*S(t_i)/max;
		return (uint16_t)s_i;
	}

	virtual ~WaveForm() {}

};
	
class SinWaveForm: public WaveForm
{
	//double amp;
	double freq;
	//double phase;

	virtual double S(double x)
	{
		return sin(2*PI*this->freq*x);
	}
	
	public:
	SinWaveForm(double freq)
	{
	//	this->amp = amp;
		this->freq = freq;
	//	this->phase = phase;
	}

};

class WaveFileChunk
{
	protected:
	std::string id;
	uint32_t size;
	public:
		uint32_t getSize() { return this->size; }
		virtual void serialize(std::ofstream& os)=0;
		virtual ~WaveFileChunk() {}
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
		this->size = 0; // IMPLEMENT SIZE COUNT
		this->type = "WAVE";
	}

	virtual void serialize(std::ofstream& os)
	{
		os << this->id; //<< this->size << this->type;

		this->size = 4;
		for(std::vector<WaveFileChunk*>::iterator i = this->chunks.begin(); i != this->chunks.end(); ++i)
			this->size += (*i)->getSize() + 8;

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
	
	uint16_t getCompression() { return this->compression; }
	uint16_t getChannels() { return this->channels; }
	uint32_t getRate() { return this->rate; }
	uint32_t getSample() { return this->sample; }
	
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

	WaveFileFormatChunk(uint16_t compression, uint16_t channels, uint32_t rate, uint16_t sample)
	{
		this->id = "fmt ";
		this->size = 16;
		this->compression = compression;
		this->channels = channels;
		this->rate = rate;
		this->bpsec = channels*rate*sample/8;
		this->align = 2;
		this->sample = sample;
	}

};


class WaveFileDataChunk: public WaveFileChunk
{
	unsigned int length;
	WaveFileFormatChunk* fmt;
	std::vector<WaveForm*> channels;	

	public:
	WaveFileDataChunk(WaveFileFormatChunk* fmt, unsigned int length):
	channels(fmt->getChannels(), NULL)
	{
		this->id = "data";
		this->fmt = fmt;
		this->length = length;
		this->size = fmt->getChannels()*fmt->getRate()*fmt->getSample()/8;
	}
	
	void setLength(unsigned int length)
	{
		this->length = length;
		this->size = this->fmt->getChannels()*this->fmt->getRate()*this->fmt->getSample()/8;
	}

	void setChannelWaveForm(unsigned int ch, WaveForm* wf)
	{
		if(ch >= fmt->getChannels()) throw 1;
	//	std::cout << "5" << std::endl;
		this->channels[ch] = wf;
	//	std::cout << "6" << std::endl;
	}
	
	virtual void serialize(std::ofstream& os)
	{
		os << this->id;
		os.write((char *)&(this->size), sizeof(this->size));
		
		for(unsigned int i = 1; i <= this->length; i++)
		{
			for(unsigned int j = 0; j < fmt->getChannels(); j++)
			{
				WaveForm* wf = this->channels[j];
				uint16_t q = (*wf)(i, this->fmt->getRate(), 1.0, 16);
				os.write((char *)&q, sizeof(q));
			}
		}

	}

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
		
	outfile.open(argv[1], ofstream::out | ofstream::binary | ofstream::trunc);

	if(outfile.fail())
	{
		std::cout << "Can not open " << argv[1] << " for writing." << std::endl;
		exit(1);
	}

	WaveFileHeader wfh;

	WaveFileFormatChunk *fmt = new WaveFileFormatChunk(1, 1, 44100, 16);
	WaveFileDataChunk *data = new WaveFileDataChunk(fmt, 44100);	
	
	wfh.add(fmt);
	wfh.add(data);

	SinWaveForm * swf = new SinWaveForm(440);

	data->setChannelWaveForm(0, swf);

	wfh.serialize(outfile);

	outfile.close();

	delete fmt;
	delete data;
	delete swf;
	
	return 0;
}

