#pragma once

#include "Component.h"
#include <memory>
#include <glm/glm.hpp>

class EnemyState;

enum class EnemyType
{
	Bee,
	Butterfly,
	BossGalaga
};

enum class EnemyStateId
{
	FlyingIntoFormation,
	InFormation,
	Diving,
	TractorBeam,
	Dead
};

class EnemyComponent final : public dae::Component
{
public:
	EnemyComponent(dae::GameObject* owner, EnemyType type);
	~EnemyComponent() override;

	void Update(float deltaTime) override;

	int GetScoreValue() const;

	EnemyType GetType() const;
	EnemyStateId GetStateId() const;

	void StartDiving();
	void StartTractorBeam();
	void ReturnToFormation();

	void FlyIntoFormation(const glm::vec3& targetPosition);
	const glm::vec3& GetFormationPosition() const;
	void SetFormationPosition(const glm::vec3& position);

	void TakeDamage();
	bool IsDead() const;
	bool IsInFormation() const;

private:
	void ChangeState(std::unique_ptr<EnemyState> newState);

	EnemyType m_Type{};
	std::unique_ptr<EnemyState> m_State{};
	int m_Health{ 1 };
	glm::vec3 m_FormationPosition{};

	bool m_IsInFormation{ true };
};