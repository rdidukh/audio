#ifndef _LINUX_PLAYBACK_DEVICE_
#define _LINUX_PLAYBACK_DEVICE_

#define ALSA_PCM_NEW_HW_PARAMS_API
#include <alsa/asoundlib.h>
#include <string>

#include "AudioDevice.h"

class LinuxPlaybackDevice: public AudioDevice
{
public:
    virtual void process();
private:

    snd_pcm_t *mHandle;
    snd_pcm_hw_params_t *mParams;
    AudioInput mInput;
    size_t
};

#if 0


	public:
	
    LinuxPlaybackDevice(const std::string & deviceName, unsigned int numChannels, unsigned int sampleRate, unsigned int sampleSize, unsigned long frames);
    virtual unsigned long writeFrames(const char * buffer, unsigned long frames);
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
        setChannelsPrivate(numChannels);
	}

    virtual void setSampleRate(unsigned int sampleRate)
    {
        setSampleRatePrivate(sampleRate);
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

private:

    void setChannelsPrivate(unsigned int numChannels)
    {
        int rc = snd_pcm_hw_params_set_channels(handle, params, numChannels);
        if(rc < 0) setFailString("LinuxPlaybackDevice::setChannelsPrivate failed");
        SoundStream::setChannels(numChannels);
    }

    void setSampleRatePrivate(unsigned int sampleRate)
    {
        int dir = 0;
        int rc = snd_pcm_hw_params_set_rate_near(handle, params, &sampleRate, &dir);
        if(rc < 0) setFail(true);
        SoundStream::setSampleRate(sampleRate);
    }

};
#endif // 0
#endif // _LINUX_PLAYBACK_DEVICE_
