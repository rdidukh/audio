#pragma once
#include <poll.h>
#include <unistd.h>

#include <vector>
#include <map>
#include <set>

#include "FDEvent.h"
#include "EventHandler.h"
#include "EventPool.h"

class FDEventPool: public EventPool
{
    public:
    void run()
    {
        while(true)
        {
            int ret = poll(&mFds[0], mFds.size(), -1);
            
            size_t i = 0;
            while(ret > 0 && i < mFds.size())
            {
                int fd = mFds[i].fd;
                bool in = mFds[i].events & POLLIN;
                bool out = mFds[i].events & POLLOUT;

                FDEvent *event = mFdToEvent[fd];

                if((event->in() && in) || (event->out() && out))
                {
                    event->handle();
                }

                ret--;
                i++;
            }

        }
    }

    void removeEvent(FDEvent * event)
    {

    }

    void addEvent(FDEvent * event)
    {
        if(mEvents.count(event) > 0)
        {
            removeEvent(event);
        }

        if(mFdToEvent.count(event->fd()) > 0)
        {
            removeEvent(mFdToEvent[event->fd()]);
        } 

        mEvents.insert(event);
        mFdToEvent[event->fd()] = event;
        struct pollfd pfd;
        pfd.fd = event->fd();
        if(event->in()) pfd.events |= POLLIN;
        if(event->out()) pfd.events |= POLLOUT;
        pfd.revents = 0;
        mFds.push_back(pfd);
    }

    private:
    std::set<FDEvent *> mEvents;
    std::map<int, FDEvent *> mFdToEvent;
    std::vector<struct pollfd> mFds;
};
