#pragma once

#include <string>
#include "EventType.h"

class IEvent
{
public:
	// events are deleted through IEvent* (EventDispatcher)
	virtual ~IEvent() = default;
	virtual const EventType GetType() const = 0;
};