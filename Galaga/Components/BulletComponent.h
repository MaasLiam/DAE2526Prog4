#pragma once

#include "Component.h"

namespace dae
{
	class Scene;
}

class BulletComponent final : public dae::Component
{
public:
	BulletComponent(dae::GameObject* owner, dae::Scene& scene, float speed);

	void Update(float deltaTime) override;

private:
	dae::Scene& m_Scene;
	float m_Speed{};
};