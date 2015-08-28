#include "WavFile.h"
#include "LinuxPlaybackDevice.h"

int main(int argc, char * argv[])
{
	assert(argc == 2);

	WavFile wf(argv[1]);

	if(wf.fail())
	{
		std::cout << wf.failString() << std::endl;
		return 1;
	}

	LinuxPlaybackDevice device("default");

	char buffer[10000*4];

	while(!wf.eof())
	{
		unsigned long frames = wf.getFrames(buffer, 10000);
		device.write(buffer, frames);
	}

	return 0;
}

