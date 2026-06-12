#pragma once

#include "Component.h"

namespace dae
{
	class Scene;
}



namespace galaga
{
	class MissileLimitComponent;
	class BulletComponent final : public dae::Component
	{
	public:
		BulletComponent(dae::GameObject* owner, dae::Scene& scene, float speed, MissileLimitComponent* missileLimit = nullptr);

		~BulletComponent() override;

		void Update(float deltaTime) override;

		BulletComponent(const BulletComponent&) = delete;
		BulletComponent(BulletComponent&&) = delete;
		BulletComponent& operator=(const BulletComponent&) = delete;
		BulletComponent& operator=(BulletComponent&&) = delete;

	private:
		dae::Scene& m_Scene;
		float m_Speed{};
		MissileLimitComponent* m_MissileLimit{};
	};
}