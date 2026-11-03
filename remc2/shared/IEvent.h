#pragma once

#include <string>
#include "EventType.h"

class IEvent
{
public:
	virtual ~IEvent() = default;
	virtual const EventType GetType() const = 0;
};