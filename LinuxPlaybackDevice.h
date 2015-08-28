#ifndef _LINUX_PLAYBACK_DEVICE_
#define _LINUX_PLAYBACK_DEVICE_

#define ALSA_PCM_NEW_HW_PARAMS_API
#include <alsa/asoundlib.h>
#include <string>

class LinuxPlaybackDevice
{
	snd_pcm_t *handle;
	snd_pcm_hw_params_t *params;

	public:
		
	LinuxPlaybackDevice(const std::string & deviceName);
	int write(char * buffer, unsigned long frames);
	~LinuxPlaybackDevice();
};

#endif // _LINUX_PLAYBACK_DEVICE_
