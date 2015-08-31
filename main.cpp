#include "WavFileSoundStream.h"
#include "LinuxPlaybackDevice.h"
#include "AudioDevice.h"

int main(int argc, char * argv[])
{
	assert(argc == 2);

    WavFileInputSoundStream waveFile(argv[1]);

    if(waveFile.fail())
	{
        std::cout << waveFile.failString() << std::endl;
		return 1;
	}

	LinuxPlaybackDevice device("default");

    if(device.fail())
    {
        std::cout << device.failString() << std::endl;
        return 1;
    }

    unsigned long frames = 1000;

    waveFile.setBufferSize(frames);
    device.setBufferSize(frames);

    SimpleAudioDevice player(waveFile, device);

    while(!player.end())
    {
        player.process();
    }

	return 0;
}

