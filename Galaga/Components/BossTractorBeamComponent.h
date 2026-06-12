#pragma once

#include "Component.h"
#include "GalagaGameControllerComponent.h"
#include "GameplayConstants.h"

namespace dae
{
	class GameObject;
	class Scene;
}

namespace galaga 
{
	class BossTractorBeamComponent final : public dae::Component
	{
	public:
		BossTractorBeamComponent(dae::GameObject* owner, dae::Scene& scene, GalagaGameControllerComponent& gameController);

		void Update(float deltaTime) override;

	private:
		void CreateBeamVisual();
		void RemoveBeamVisual();
		void UpdateBeamVisualPosition();
		bool TryCapturePlayer(galaga::PlayerIndex playerIndex);
		void SpawnCapturedFighter(galaga::PlayerIndex playerIndex);

		dae::Scene& m_Scene;
		GalagaGameControllerComponent& m_GameController;
		dae::GameObject* m_BeamVisual{};
		bool m_HasCapturedThisBeam{};

		static inline const glm::vec3 BeamVisualOffset{ galaga::gameplay::TractorBeamVisualOffset };
		static constexpr float BeamHitboxLeftOffset{ galaga::gameplay::TractorBeamHitboxLeftOffset };
		static constexpr float BeamHitboxTopOffset{ galaga::gameplay::TractorBeamHitboxTopOffset };
		static constexpr float BeamHitboxWidth{ galaga::gameplay::TractorBeamHitboxWidth };
		static constexpr float BeamHitboxHeight{ galaga::gameplay::TractorBeamHitboxHeight };
	};
}