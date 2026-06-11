#pragma once

#include "Component.h"

#include <glm/vec3.hpp>

namespace dae
{
	class GameObject;
}

class GalagaGameControllerComponent;

class VersusBossComponent final : public dae::Component
{
public:
	VersusBossComponent(dae::GameObject* owner, GalagaGameControllerComponent& gameController, dae::GameObject& targetPlayer);

	void Update(float deltaTime) override;

	void StartDive();
	void StartTractorBeam();
	void TakeHit();
	bool IsDead() const;
	void SetBeamVisual(dae::GameObject* beamVisual);

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

	glm::vec3 m_StartPosition{ 400.f, 90.f, 0.f };
	glm::vec3 m_BeamPosition{ 400.f, 350.f, 0.f };
	glm::vec3 m_BeamVisualOffset{ -15.f, 32.f, 0.f };
	glm::vec3 m_DiveTarget{ 400.f, 500.f, 0.f };

	float m_BeamHitboxLeftOffset{ -15.f };
	float m_BeamHitboxTopOffset{ 32.f };
	float m_BeamHitboxWidth{ 64.f };
	float m_BeamHitboxHeight{ 300.f };

	float m_TractorTimer{};
	bool m_HasDamagedPlayerThisAttack{};
	int m_HitPoints{ 4 };
	bool m_DamagedVisualApplied{};
	dae::GameObject* m_BeamVisual{};
};