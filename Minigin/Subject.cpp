#include "Subject.h"
#include "Observer.h"

namespace dae
{
	void Subject::AddObserver(Observer* observer)
	{
		m_Observers.push_back(observer);
	}
	void Subject::RemoveObserver(Observer* observer)
	{
		m_Observers.erase(
			std::remove(m_Observers.begin(), m_Observers.end(), observer), 
			m_Observers.end());
	}
	void Subject::Notify(Event event, GameObject* sender)
	{
		for (Observer* observer : m_Observers)
		{
			observer->Notify(event, sender);
		}
	}
}