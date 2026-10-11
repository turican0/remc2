#pragma once

#include <map>
#include <vector>
#include "Event.h"

class EventDispatcher
{
public:
	static EventDispatcher* I;

	~EventDispatcher()
	{
		for (auto el : _eventList)
		{
			for (auto e : el.second)
				delete[] e;
		}
	};

	void RegisterEvent(IEvent* event)
	{
		if (event)
			_eventList[event->GetType()].push_back(event);
	};

	template <typename ..._args>
	void DispatchEvent(EventType eventType, _args...a)
	{
		auto it_eventList = _eventList.find(eventType);
		if (it_eventList == _eventList.end())
			return;

		for (auto ie : it_eventList->second)
		{
			if (Event<_args...>* event = dynamic_cast<Event<_args...>*>(ie))
				event->Trigger(a...);
		}
	};

	template <typename ..._args>
	void UnregisterEvent(EventType eventType, void(*func)(_args...))
	{
		auto it_eventList = _eventList.find(eventType);
		if (it_eventList == _eventList.end())
			return;

		auto& events = it_eventList->second;
		for (auto it = events.begin(); it != events.end(); ++it)
		{
			if (Event<_args...>* event = dynamic_cast<Event<_args...>*>(*it))
			{
				using FuncPtr = void(*)(_args...);
				const FuncPtr* target = event->GetCallback().template target<FuncPtr>();
				if (target && *target == func)
				{
					delete event;
					events.erase(it);
					return;
				}
			}
		}
	};

private:
	std::map<EventType, std::vector<IEvent*>> _eventList;
};