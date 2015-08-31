#include "WavFileSoundStream.h"
#include "LinuxPlaybackDevice.h"
#include "AudioDevice.h"

int main(int argc, char * argv[])
{
	assert(argc == 2);

    unsigned long frames = 10000;

    WavFileInputSoundStream waveFile(argv[1]);

    if(waveFile.fail())
	{
        std::cout << waveFile.failString() << std::endl;
		return 1;
	}

    std::cout << "channels = " << waveFile.channels() << std::endl;
    std::cout << "sampleRate = " << waveFile.sampleRate() << std::endl;
//    std::cout <<

    LinuxPlaybackDevice device("default", waveFile.channels(), waveFile.sampleRate(), waveFile.sampleSize(), frames);

    if(device.fail())
    {
        std::cout << device.failString() << std::endl;
        return 1;
    }

    waveFile.setBufferSize(frames);
   // device.setBufferSize(frames);

    SimpleAudioDevice player(waveFile, device);

    while(!player.end())
    {
        player.process();
    }

	return 0;
}

