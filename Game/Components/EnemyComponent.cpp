#include "EnemyComponent.h"

EnemyComponent::EnemyComponent(dae::GameObject* owner, EnemyType type)
	: dae::Component(owner)
	, m_Type(type)
{
	if (m_Type == EnemyType::BossGalaga)
		m_Health = 2;
}

int EnemyComponent::GetScoreValue() const
{
	switch (m_Type)
	{
	case EnemyType::Bee:
		return m_State == EnemyState::Diving ? 100 : 50;

	case EnemyType::Butterfly:
		return m_State == EnemyState::Diving ? 160 : 80;

	case EnemyType::BossGalaga:
		return m_State == EnemyState::Diving ? 400 : 150;
	}

	return 0;
}

EnemyType EnemyComponent::GetType() const
{
	return m_Type;
}

EnemyState EnemyComponent::GetState() const
{
	return m_State;
}

void EnemyComponent::SetState(EnemyState state)
{
	m_State = state;
}

void EnemyComponent::TakeDamage()
{
	--m_Health;
}

bool EnemyComponent::IsDead() const
{
	return m_Health <= 0;
}