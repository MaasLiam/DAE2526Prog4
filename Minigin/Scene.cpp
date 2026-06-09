#include <algorithm>
#include "Scene.h"
#include <assert.h>

using namespace dae;

void Scene::Add(std::unique_ptr<GameObject> object)
{
	assert(object != nullptr && "Cannot add a null GameObject to the scene.");

	if (m_IsUpdating)
	{
		m_objectsToAdd.emplace_back(std::move(object));
		return;
	}

	m_objects.emplace_back(std::move(object));
}

void Scene::Remove(const GameObject& object)
{
	m_objectsToRemove.push_back(&object);
}

void Scene::RemoveAll()
{
	if (m_IsUpdating)
	{
		for (const auto& object : m_objects)
		{
			m_objectsToRemove.push_back(object.get());
		}

		m_objectsToAdd.clear();
		return;
	}

	m_objects.clear();
	m_objectsToAdd.clear();
	m_objectsToRemove.clear();
}

const std::vector<std::unique_ptr<GameObject>>& dae::Scene::GetObjects() const
{
	return m_objects;
}

void Scene::Update(float deltaTime)
{
	m_IsUpdating = true;

	for (auto& object : m_objects)
	{
		object->Update(deltaTime);
	}

	m_IsUpdating = false;
}

void dae::Scene::LateUpdate()
{
	for (auto const objectToRemove : m_objectsToRemove)
	{
		m_objects.erase(
			std::remove_if(
				m_objects.begin(),
				m_objects.end(),
				[objectToRemove](const auto& ptr)
				{
					return ptr.get() == objectToRemove;
				}
			),
			m_objects.end()
		);
	}

	m_objectsToRemove.clear();

	for (auto& objectToAdd : m_objectsToAdd)
	{
		m_objects.emplace_back(std::move(objectToAdd));
	}

	m_objectsToAdd.clear();
}
void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

