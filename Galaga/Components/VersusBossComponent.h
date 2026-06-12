#pragma once

#include "Component.h"

#include <glm/vec3.hpp>
#include "GameplayConstants.h"

namespace dae
{
	class GameObject;
}

namespace galaga
{
	class GalagaGameControllerComponent;
	class VersusBossComponent final : public dae::Component
	{
	public:
		VersusBossComponent(dae::GameObject* owner, GalagaGameControllerComponent& gameController, dae::GameObject& targetPlayer);

		~VersusBossComponent() override = default;

		VersusBossComponent(const VersusBossComponent&) = delete;
		VersusBossComponent(VersusBossComponent&&) = delete;
		VersusBossComponent& operator=(const VersusBossComponent&) = delete;
		VersusBossComponent& operator=(VersusBossComponent&&) = delete;

		void Update(float deltaTime) override;
		void StartDive();
		void StartTractorBeam();
		void TakeHit();
		bool IsDead() const;
		void SetBeamVisual(dae::GameObject* beamVisual);
		void Reset();

	private:
		enum class BossState
		{
			Idle,
			DivingToPlayer,
			ExitingBottom,
			Returning,
			MovingToBeamPosition,
			TractorBeam,
			ReturningFromBeam
		};

		void MoveTowards(const glm::vec3& target, float speed, float deltaTime);
		bool IsNear(const glm::vec3& target, float distance) const;
		void TryDamagePlayerOnContact();
		void TryCapturePlayerWithBeam();
		void SetBeamVisible(bool isVisible);

		GalagaGameControllerComponent& m_GameController;
		dae::GameObject& m_TargetPlayer;

		BossState m_State{ BossState::Idle };

		glm::vec3 m_StartPosition{ galaga::gameplay::VersusBossStartPosition };
		glm::vec3 m_BeamPosition{ galaga::gameplay::VersusBossStartX, galaga::gameplay::TractorBeamY, 0.f };
		glm::vec3 m_BeamVisualOffset{ galaga::gameplay::TractorBeamVisualOffset };
		glm::vec3 m_DiveTarget{ galaga::gameplay::VersusBossStartX, galaga::gameplay::PlayerStartY, 0.f };

		float m_BeamHitboxLeftOffset{ galaga::gameplay::TractorBeamHitboxLeftOffset };
		float m_BeamHitboxTopOffset{ galaga::gameplay::TractorBeamHitboxTopOffset };
		float m_BeamHitboxWidth{ galaga::gameplay::TractorBeamHitboxWidth };
		float m_BeamHitboxHeight{ galaga::gameplay::TractorBeamHitboxHeight };

		float m_TractorTimer{};
		bool m_HasDamagedPlayerThisAttack{};
		int m_HitPoints{ 4 };
		bool m_DamagedVisualApplied{};
		dae::GameObject* m_BeamVisual{};
	};
}