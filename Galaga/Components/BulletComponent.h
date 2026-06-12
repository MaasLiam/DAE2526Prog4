#pragma once

#include "Component.h"

namespace dae
{
	class Scene;
	class TransformComponent;
}

namespace galaga
{
	class MissileLimitComponent;

	class BulletComponent final : public dae::Component
	{
	public:
		BulletComponent(dae::GameObject* owner, dae::Scene& scene, float speed, MissileLimitComponent* missileLimit = nullptr);
		~BulletComponent() override;

		BulletComponent(const BulletComponent&) = delete;
		BulletComponent(BulletComponent&&) = delete;
		BulletComponent& operator=(const BulletComponent&) = delete;
		BulletComponent& operator=(BulletComponent&&) = delete;

		void Update(float deltaTime) override;

	private:
		dae::Scene& m_Scene;
		dae::TransformComponent* m_Transform{};
		MissileLimitComponent* m_MissileLimit{};
		float m_Speed{};
	};
}