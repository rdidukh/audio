#include "LinuxPlaybackDevice.h"

LinuxPlaybackDevice::LinuxPlaybackDevice(const std::string & deviceName):
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

	/* Signed 16-bit little-endian format */
	snd_pcm_hw_params_set_format(handle, params,                 SND_PCM_FORMAT_S16_LE);

	/* Two channels (stereo) */
	snd_pcm_hw_params_set_channels(handle, params, 2);

	unsigned int sampleRate = 44100;
	snd_pcm_hw_params_set_rate_near(handle, params, &sampleRate, &dir);

	  /* Set period size to 32 frames. */
	snd_pcm_uframes_t frames = 10000;
	snd_pcm_hw_params_set_period_size_near(handle, params, &frames, &dir);

	/* Write the parameters to the driver */
	rc = snd_pcm_hw_params(handle, params);
	if (rc < 0) throw("1");

	/* Use a buffer large enough to hold one period */	
	snd_pcm_hw_params_get_period_size(params, &frames,
                                &dir);
	// int size = frames * 4; /* 2 bytes/sample, 2 channels */
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
}

