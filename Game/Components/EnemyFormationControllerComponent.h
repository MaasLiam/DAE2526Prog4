#pragma once

#include "Component.h"
#include <vector>

class EnemyComponent;

class EnemyFormationControllerComponent final : public dae::Component
{
public:
	EnemyFormationControllerComponent(dae::GameObject* owner);

	void Update(float deltaTime) override;

	void AddEnemy(EnemyComponent* enemy);

private:
	std::vector<EnemyComponent*> m_Enemies{};

	float m_DiveTimer{};
	float m_TimeBetweenDives{ 3.f };
};