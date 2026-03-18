#pragma once

namespace dae
{
	enum class Event;
	class GameObject;
	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void Notify(Event event, GameObject* sender) = 0;
	};
}