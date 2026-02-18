#include <algorithm>
#include "Scene.h"
#include <assert.h>

using namespace dae;

void Scene::Add(std::unique_ptr<GameObject> object)
{
	assert(object != nullptr && "Cannot add a null GameObject to the scene.");
	m_objects.emplace_back(std::move(object));
}

void Scene::Remove(const GameObject& object)
{
	m_objectsToRemove.push_back(&object);
}

void Scene::RemoveAll()
{
	m_objects.clear();
}

void Scene::Update(float deltaTime)
{
	for(auto& object : m_objects)
	{

		object->Update(deltaTime);
	}
}

void dae::Scene::LateUpdate()
{
	for (auto const objectToRemove : m_objectsToRemove)
	{
		m_objects.erase(
			std::remove_if(
				m_objects.begin(),
				m_objects.end(),
				[objectToRemove](const auto& ptr) { return ptr.get() == objectToRemove; }
			),
			m_objects.end()
		);
	}
	m_objectsToRemove.clear();
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

