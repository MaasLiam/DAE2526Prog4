#pragma once

#include "Component.h"

enum class EnemyType
{
	Bee,
	Butterfly,
	BossGalaga
};

enum class EnemyState
{
	InFormation,
	Diving
};

class EnemyComponent final : public dae::Component
{
public:
	EnemyComponent(dae::GameObject* owner, EnemyType type);

	int GetScoreValue() const;

	EnemyType GetType() const;
	EnemyState GetState() const;

	void SetState(EnemyState state);

	void TakeDamage();
	bool IsDead() const;

private:
	EnemyType m_Type{};
	EnemyState m_State{ EnemyState::InFormation };
	int m_Health{ 1 };
};