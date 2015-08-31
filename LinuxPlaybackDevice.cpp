#include "LinuxPlaybackDevice.h"

LinuxPlaybackDevice::LinuxPlaybackDevice(const std::string & deviceName, unsigned int numChannels, unsigned int sampleRate, unsigned int frames):
handle(nullptr),
params(nullptr)
{
	int rc;
	int dir;

	rc = snd_pcm_open(&handle, deviceName.c_str(),
                SND_PCM_STREAM_PLAYBACK, 0);		

	if(rc < 0) throw std::string(snd_strerror(rc));

	snd_pcm_hw_params_alloca(&params);

	snd_pcm_hw_params_any(handle, params);

	snd_pcm_hw_params_set_access(handle, params,
                  SND_PCM_ACCESS_RW_INTERLEAVED);

    setFormat();

	setChannels(numChannels);

	setSampleRate(sampleRate);

	setFrames(frames);

	setParams();
}	

int LinuxPlaybackDevice::write(char * buffer, unsigned long frames)
{
	int rc = snd_pcm_writei(handle, buffer, frames);
	if (rc == -EPIPE)
	{
		/* EPIPE means underrun */
		fprintf(stderr, "underrun occurred\n");
		snd_pcm_prepare(handle);
	}
	else if (rc < 0)
	{
		fprintf(stderr, "error from writei: %s\n", snd_strerror(rc));
	}
	else if (rc != (int)frames)
	{
		fprintf(stderr,
          "short write, write %d frames\n", rc);
	}
	return rc;
}

LinuxPlaybackDevice::~LinuxPlaybackDevice()
{
	if(handle)
	{
		snd_pcm_drain(handle);
		snd_pcm_close(handle);
	}

	if(params)
	{
		snd_pcm_hw_params_free(params);
	}
}

