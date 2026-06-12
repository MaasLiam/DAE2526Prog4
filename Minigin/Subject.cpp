#include "Subject.h"

#include "Observer.h"

#include <algorithm>

dae::Subject::~Subject()
{
	for (auto* observer : m_Observers)
	{
		if (observer == nullptr)
		{
			continue;
		}

		observer->OnSubjectDestroyed(this);
	}
}

void dae::Subject::AddObserver(Observer* observer)
{
	if (observer == nullptr)
	{
		return;
	}

	const auto isAlreadyRegistered = std::find(
		m_Observers.begin(),
		m_Observers.end(),
		observer
	) != m_Observers.end();

	if (isAlreadyRegistered)
	{
		return;
	}

	m_Observers.emplace_back(observer);
}

void dae::Subject::RemoveObserver(Observer* observer)
{
	m_Observers.erase(
		std::remove(m_Observers.begin(), m_Observers.end(), observer),
		m_Observers.end()
	);
}

void dae::Subject::Notify(Event event, GameObject* sender)
{
	for (auto* observer : m_Observers)
	{
		if (observer == nullptr)
		{
			continue;
		}

		observer->Notify(event, sender);
	}
}