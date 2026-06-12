#include "Scene.h"

#include "GameObject.h"

#include <algorithm>

void dae::Scene::Add(std::unique_ptr<GameObject> object)
{
	if (object == nullptr)
	{
		return;
	}

	if (m_IsUpdating)
	{
		m_ObjectsToAdd.emplace_back(std::move(object));
		return;
	}

	m_Objects.emplace_back(std::move(object));
}

void dae::Scene::Remove(const GameObject& object)
{
	const auto alreadyMarkedForRemoval = std::find(m_ObjectsToRemove.begin(), m_ObjectsToRemove.end(), &object) != m_ObjectsToRemove.end();

	if (alreadyMarkedForRemoval)
	{
		return;
	}

	m_ObjectsToRemove.emplace_back(&object);
}

void dae::Scene::RemoveAll()
{
	m_Objects.clear();
	m_ObjectsToAdd.clear();
	m_ObjectsToRemove.clear();
}

const std::vector<std::unique_ptr<dae::GameObject>>& dae::Scene::GetObjects() const
{
	return m_Objects;
}

void dae::Scene::Update(float deltaTime)
{
	m_IsUpdating = true;

	for (const auto& object : m_Objects)
	{
		object->Update(deltaTime);
	}

	m_IsUpdating = false;

	RemovePendingObjects();
	AddPendingObjects();
}

void dae::Scene::LateUpdate()
{}

void dae::Scene::Render() const
{
	for (const auto& object : m_Objects)
	{
		object->Render();
	}
}

void dae::Scene::AddPendingObjects()
{
	if (m_ObjectsToAdd.empty())
	{
		return;
	}

	for (auto& object : m_ObjectsToAdd)
	{
		m_Objects.emplace_back(std::move(object));
	}

	m_ObjectsToAdd.clear();
}

void dae::Scene::RemovePendingObjects()
{
	if (m_ObjectsToRemove.empty())
	{
		return;
	}

	m_Objects.erase(
		std::remove_if(
			m_Objects.begin(),
			m_Objects.end(),
			[this](const std::unique_ptr<GameObject>& object)
			{
				return std::find(
					m_ObjectsToRemove.begin(),
					m_ObjectsToRemove.end(),
					object.get()
				) != m_ObjectsToRemove.end();
			}
		),
		m_Objects.end()
	);

	m_ObjectsToRemove.clear();
}