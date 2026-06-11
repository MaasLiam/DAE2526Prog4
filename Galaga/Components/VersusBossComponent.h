#pragma once

#include "Component.h"

#include <glm/vec3.hpp>

class GalagaGameControllerComponent;

class VersusBossComponent final : public dae::Component
{
public:
	VersusBossComponent(dae::GameObject* owner, GalagaGameControllerComponent& gameController);

	void Update(float deltaTime) override;
	void StartDive();
	void StartTractorBeam();

private:
	enum class BossState
	{
		Idle,
		Diving,
		Returning,
		TractorBeam
	};

	GalagaGameControllerComponent& m_GameController;

	BossState m_State{ BossState::Idle };
	glm::vec3 m_StartPosition{ 460.f, 90.f, 0.f };
	float m_TractorTimer{};
};