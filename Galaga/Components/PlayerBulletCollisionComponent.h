#pragma once

#include "Component.h"

namespace dae
{
	class GameObject;
	class Scene;
}

namespace galaga
{
	class PlayerBulletCollisionComponent final : public dae::Component
	{
	public:
		PlayerBulletCollisionComponent(dae::GameObject* owner, dae::Scene& scene, dae::GameObject& player);

		void Update(float deltaTime) override;

	private:
		dae::Scene& m_Scene;
		dae::GameObject& m_Player;
	};
}