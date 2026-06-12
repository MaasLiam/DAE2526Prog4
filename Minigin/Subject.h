#pragma once

#include "Event.h"

#include <vector>

namespace dae
{
	class GameObject;
	class Observer;

	class Subject final
	{
	public:
		Subject() = default;
		~Subject();

		Subject(const Subject&) = delete;
		Subject(Subject&&) = delete;
		Subject& operator=(const Subject&) = delete;
		Subject& operator=(Subject&&) = delete;

		void AddObserver(Observer* observer);
		void RemoveObserver(Observer* observer);
		void Notify(Event event, GameObject* sender);

	private:
		std::vector<Observer*> m_Observers{};
	};
}