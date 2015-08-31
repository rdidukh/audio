#ifndef _LINUX_PLAYBACK_DEVICE_
#define _LINUX_PLAYBACK_DEVICE_

#define ALSA_PCM_NEW_HW_PARAMS_API
#include <alsa/asoundlib.h>
#include <string>

#include "SoundStream.h"

class LinuxPlaybackDevice: public OutputSoundStream
{
	snd_pcm_t *handle;
	snd_pcm_hw_params_t *params;

	public:
	
	LinuxPlaybackDevice(const std::string & deviceName);
    unsigned int writeFrames(char * buffer, unsigned long frames) throw(SoundStreamException);
	~LinuxPlaybackDevice();

	void setFormat(unsigned int bits)
	{
		snd_pcm_format_t format;

		if(bits == 8)
			format = SND_PCM_FORMAT_S8;
		else if(bits == 16)
			format = SND_PCM_FORMAT_S16_LE;
		else
			throw "setFormat: unsupported value.";

		int rc = snd_pcm_hw_params_set_format(handle, params, format);	
		if(rc < 0) throw "setFormat";
	}

    virtual void setChannels(unsigned int numChannels)
	{
		int rc = snd_pcm_hw_params_set_channels(handle, params, numChannels);
        if(rc < 0) setFailString("LinuxPlaybackDevice::setChannels failed");
	}

	void setSampleRate(unsigned int sampleRate)
	{
		int dir = 0;
		int rc = snd_pcm_hw_params_set_rate_near(handle, params, &sampleRate, &dir);
        if(rc < 0) setFail(true);
	}

	void setFrames(unsigned int frames)
	{
		int dir = 0;
		snd_pcm_uframes_t f = static_cast<snd_pcm_uframes_t>(frames);
		int rc = snd_pcm_hw_params_set_period_size_near(handle, params, &f, &dir);
		if(rc < 0) throw("setFrames");	
	}

	unsigned int getFrames()
	{
		int dir = 0;
		snd_pcm_uframes_t frames;
	
		int rc = snd_pcm_hw_params_get_period_size(params, &frames, &dir);	
			
		if (rc < 0) throw("getFrames");	
	
		return static_cast<unsigned int>(frames);
	}

	void setParams()
	{
		int rc = snd_pcm_hw_params(handle, params);
		if (rc < 0) throw("setParams");
	}
};

#endif // _LINUX_PLAYBACK_DEVICE_
