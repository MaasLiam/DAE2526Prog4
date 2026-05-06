#include "EnemyFormationControllerComponent.h"

#include "EnemyComponent.h"
#include <cstdlib>

EnemyFormationControllerComponent::EnemyFormationControllerComponent(dae::GameObject* owner)
	: dae::Component(owner)
{
}

void EnemyFormationControllerComponent::Update(float deltaTime)
{
	m_DiveTimer += deltaTime;

	if (m_DiveTimer < m_TimeBetweenDives)
		return;

	m_DiveTimer = 0.f;

	std::vector<EnemyComponent*> availableEnemies{};

	for (auto* enemy : m_Enemies)
	{
		if (!enemy)
			continue;

		if (enemy->IsDead())
			continue;

		if (enemy->GetStateId() == EnemyStateId::InFormation)
			availableEnemies.push_back(enemy);
	}

	if (availableEnemies.empty())
		return;

	const int randomIndex = rand() % static_cast<int>(availableEnemies.size());
	availableEnemies[randomIndex]->StartDiving();
}

void EnemyFormationControllerComponent::AddEnemy(EnemyComponent* enemy)
{
	m_Enemies.push_back(enemy);
}