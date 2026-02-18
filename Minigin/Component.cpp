#include "Component.h"
#pragma once

dae::Component::Component(GameObject* parent)
	: m_Parent(parent)
{
}

void dae::Component::Update(float deltaTime)
{
	deltaTime;
}

void dae::Component::Render() const
{
}
