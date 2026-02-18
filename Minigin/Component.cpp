#include "Component.h"

dae::Component::Component(GameObject* parent)
	: m_Parent(parent)
{
}

void dae::Component::Update(float deltaTime)
{
	(void)deltaTime;
}

void dae::Component::Render() const
{
}
