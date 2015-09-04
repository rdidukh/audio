#pragma once

#include "Event.h"

class EventHandler
{
public:
    virtual void handleEvent(Event * event) = 0;

};
