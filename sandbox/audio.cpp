#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <stdint.h> // for uint32_t
#include <vector>
//#include <pair>

using namespace std;

static const double PI = 3.14159265;

class WaveForm
{
public:
	virtual double S(double x)=0;
	
	double operator()(double x)
	{
		return S(x);
	}
	
	uint16_t operator()(unsigned int t, unsigned int rate, double max, int bits)
	{
		double t_i = (double)t/rate;
		double s_i = S(t_i);
	//	if(s_i > max) s_i = max;
	//	if(s_i < -max) s_i = -max;
		s_i = ((1<<(bits-1))-1)*s_i/max;
		return (uint16_t)s_i;
	}

	virtual ~WaveForm() {}

};
	
class SinWaveForm: public WaveForm
{
	//double amp;
	double freq;
	//double phase;
public:
	virtual double S(double x)
	{
		return sin(2*PI*this->freq*x);
	}
	
	
	SinWaveForm(double freq)
	{
	//	this->amp = amp;
		this->freq = freq;
	//	this->phase = phase;
	}

};


class SquareWaveForm: public WaveForm
{
	//double amp;
	double freq;
	double width;

public:
	virtual double S(double x)
	{
		double temp = x*freq;	
		temp = temp - floor(temp);

		if(temp <= 0.5)
		{
			return 2*temp <= width ? 1 : 0;
		}
		else
		{
			return 2*temp-1 <= width ? -1 : 0;
		}	
	
		
		//return (temp<=0.5) ? 1 : -1;
	}
	
	
	SquareWaveForm(double freq)
	{
	//  0 < width < 1
		
	//	this->amp = amp;
		this->freq = freq;
		this->width = 1;
	//	this->width = width;
	//	this->phase = phase;
	}

	SquareWaveForm(double freq, double width)
	{
	//  0 < width < 1
		
	//	this->amp = amp;
		this->freq = freq;
		this->width = width;
	//	this->width = width;
	//	this->phase = phase;
	}

};

class WaveFormMixer: public WaveForm
{
	std::vector<std::pair<WaveForm*, double> > wfs;

	double coef;
public:
	virtual double S(double x)
	{
		double result = 0;

		for(std::vector<std::pair<WaveForm*, double> >::iterator i = wfs.begin(); i != wfs.end(); ++i)
		{
			result += (*(i->first))(x) * i->second;
		}
		
		return this->coef*result;
	}
	
	WaveFormMixer()
	{
		this->coef = 1.0;
	}
	
	void add(WaveForm* wf, double amp = 1.0)
	{
		wfs.push_back(std::make_pair<WaveForm*, double>(wf, amp));
	}

	void norm(unsigned int rate, unsigned int start, unsigned int end, double norm=1.0)
	{
		double max = S((double)start/rate);

		for(unsigned int i = start+1; i < end; i++)
		{
			double s_i = S((double)i/rate);
			if(max < abs(s_i)) max = s_i;
		}
		
		if(norm != 0) this->coef = norm/max; else this->coef = 1.0;
	}

	void clear()
	{
		this->wfs.clear();
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
		this->size = length*fmt->getChannels()*fmt->getSample()/8;
		std::cout << this->size << std::endl;
	}
	
	void setLength(unsigned int length)
	{
		this->length = length;
		this->size = length*this->fmt->getChannels()*this->fmt->getSample()/8;
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
				
				if(wf == NULL) throw std::string("WaveForm is NULL");
	
				uint16_t q = (*wf)(i, this->fmt->getRate(), 1.0, 16);
				os.write((char *)&q, sizeof(q));
			}
		}
	}

};


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

	WaveFileFormatChunk* fmt = new WaveFileFormatChunk(1, 1, 44100, 16);
	WaveFileDataChunk* data = new WaveFileDataChunk(fmt, 44100);	
	
	wfh.add(fmt);
	wfh.add(data);

	SinWaveForm* swf0 = new SinWaveForm(440);
	//SquareWaveForm * swf0 = new SquareWaveForm(440, 1);
	SinWaveForm* swf1 = new SinWaveForm(554);
	SinWaveForm* swf2 = new SinWaveForm(659);

	WaveFormMixer *wfmx = new WaveFormMixer();

	wfmx->add(swf0, 0.1);
	wfmx->add(swf1, 0.9);
	wfmx->add(swf2, 0.1);
	
	for(unsigned int i = 1; i < 400; i++)
	//	std::cout << i << " " << swf0->S((double)i/44100) << " " << swf1->S((double)i/44100) << " " << swf2->S((double)i/44100) << " " << wfmx->S((double)i/44100) << std::endl;
	std::cout << std::endl;
	
	wfmx->norm(44100, 0, 44100/440, 0.95);
		
	for(unsigned int i = 1; i < 450; i++)
		std::cout << i << " " << (*wfmx)((double)i/44100) << " " << std::hex << (*wfmx)(i, 44100, 1.0, 16) << std::dec << std::endl;
	std::cout << std::endl;
	
		
	data->setChannelWaveForm(0, wfmx);
	//data->setChannelWaveForm(1, swf1);

	wfh.serialize(outfile);

	outfile.close();

	delete fmt;
	delete data;
	delete swf0;
	delete swf1;	
	delete wfmx;

	return 0;
}


