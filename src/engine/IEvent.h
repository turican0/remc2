#pragma once

#include <string>
#include "EventType.h"

class IEvent
{
public:
	virtual EventType GetType() const = 0;
	virtual ~IEvent() {};
};
