#pragma once

#include "Component.h"
#include <memory>
#include <glm/glm.hpp>

class EnemyState;

namespace galaga
{
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
		EnemyComponent(dae::GameObject* owner, galaga::EnemyType type);
		~EnemyComponent() override;
		EnemyComponent(const EnemyComponent&) = delete;
		EnemyComponent(EnemyComponent&&) = delete;
		EnemyComponent& operator=(const EnemyComponent&) = delete;
		EnemyComponent& operator=(EnemyComponent&&) = delete;

		void Update(float deltaTime) override;

		int GetScoreValue() const;

		galaga::EnemyType GetType() const;
		galaga::EnemyStateId GetStateId() const;

		void StartDiving();
		void StartTractorBeam();
		void ReturnToFormation();

		void FlyIntoFormation(const glm::vec3& targetPosition);
		const glm::vec3& GetFormationPosition() const;
		void SetFormationPosition(const glm::vec3& position);

		void TakeDamage();
		bool IsDead() const;
		bool IsInFormation() const;
		bool IsTractorBeamActive() const;
		void SetTractorBeamActive(bool isActive);

	private:
		void ChangeState(std::unique_ptr<EnemyState> newState);

		galaga::EnemyType m_Type{};
		std::unique_ptr<EnemyState> m_State{};
		int m_Health{ 1 };
		glm::vec3 m_FormationPosition{};
		bool m_IsInFormation{ true };
		bool m_IsTractorBeamActive{};
	};
}

