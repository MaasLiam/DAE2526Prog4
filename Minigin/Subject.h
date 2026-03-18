#pragma once
#include <vector>

namespace dae
{
	enum class Event;
	class Observer;
	class Subject
	{
	public:
		void AddObserver(class Observer* observer);
		void RemoveObserver(class Observer* observer);
		void Notify(Event event, class GameObject* sender);
	private:
		std::vector<class Observer*> m_Observers;
	};
}