#pragma once

#include <functional>
#include "IEvent.h"

template <typename ..._args>
class Event : public IEvent
{
public:
	//Alias for template callback
	using _callback = std::function<void(_args...)>;

	explicit Event(const EventType eventType, _callback& cb) : m_EventType(eventType), m_cbFunc(cb) {}

	~Event() {}

	//Accessors
	virtual const EventType GetType() const override { return this->m_EventType; }

	//Methods
	void Trigger(_args... a) { this->m_cbFunc(a...); }
	const _callback& GetCallback() const { return this->m_cbFunc; }

private:
	//Event identifier
	EventType m_EventType;

	//Event callback - can't be changed inside event.
	_callback  const m_cbFunc;
};