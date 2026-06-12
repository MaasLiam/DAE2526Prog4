#pragma once

#include "Event.h"

namespace dae
{
	class GameObject;
	class Subject;

	class Observer
	{
	public:
		virtual ~Observer() = default;

		virtual void Notify(Event event, GameObject* gameObject) = 0;

		virtual void OnSubjectDestroyed(Subject* subject)
		{
			(void)subject;
		}
	};
}