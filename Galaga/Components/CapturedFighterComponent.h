#pragma once

#include "Component.h"

namespace dae
{
	class Scene;
}

namespace galaga
{
	class GalagaGameControllerComponent;
	class CapturedFighterComponent final : public dae::Component
	{
	public:
		CapturedFighterComponent(dae::GameObject* owner, dae::Scene& scene, GalagaGameControllerComponent& gameController);

		void Update(float deltaTime) override;

		~CapturedFighterComponent() override = default;

		CapturedFighterComponent(const CapturedFighterComponent&) = delete;
		CapturedFighterComponent(CapturedFighterComponent&&) = delete;
		CapturedFighterComponent& operator=(const CapturedFighterComponent&) = delete;
		CapturedFighterComponent& operator=(CapturedFighterComponent&&) = delete;

	private:
		void Shoot();

		dae::Scene& m_Scene;
		GalagaGameControllerComponent& m_GameController;
		float m_ShootTimer{};
	};
}