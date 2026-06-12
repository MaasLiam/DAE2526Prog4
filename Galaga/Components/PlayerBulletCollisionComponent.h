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

		~PlayerBulletCollisionComponent() override = default;

		PlayerBulletCollisionComponent(const PlayerBulletCollisionComponent&) = delete;
		PlayerBulletCollisionComponent(PlayerBulletCollisionComponent&&) = delete;
		PlayerBulletCollisionComponent& operator=(const PlayerBulletCollisionComponent&) = delete;
		PlayerBulletCollisionComponent& operator=(PlayerBulletCollisionComponent&&) = delete;

	private:
		dae::Scene& m_Scene;
		dae::GameObject& m_Player;
	};
}