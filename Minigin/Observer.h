#pragma once

#include "Event.h"

namespace dae
{
	class GameObject;
	class Subject;

	class Observer
	{
	public:
		Observer() = default;
		virtual ~Observer() = default;

		Observer(const Observer&) = delete;
		Observer(Observer&&) = delete;
		Observer& operator=(const Observer&) = delete;
		Observer& operator=(Observer&&) = delete;

		virtual void Notify(Event event, GameObject* gameObject) = 0;

		virtual void OnSubjectDestroyed(Subject* subject)
		{
			(void)subject;
		}
	};
}