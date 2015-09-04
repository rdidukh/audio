#include <unistd.h>

#include <cstdint>

#include "Event.h"
#include "EventHandler.h"

class FDEvent: public Event
{
public:
    FDEvent(int fd, bool in = true, bool out = false)
    {
        mFd = fd;    
        mIn = in;
        mOut = out;
    }

    int fd()
    {
        return mFd;
    }

    void handle()
    {
        int64_t val;
        read(mFd, &val, sizeof(val));
        eventHandler()->handleEvent(this);
    }

    void wait(EventHandler * eventHandler, bool in = true, bool out = false)
    {
        mEventHandler = eventHandler;
        mIn = in;
        mOut = out;
    }

    void signal()
    {
        int64_t val = 1;
        write(mFd, &val, sizeof(val));
    }

    bool in()
    {
       return mIn;
    }

    bool out()
    {
       return mOut;
    }

    EventHandler * eventHandler()
    {
        return mEventHandler;
    }  

private:

    int mFd;
    bool mIn, mOut;
    EventHandler * mEventHandler;
};
