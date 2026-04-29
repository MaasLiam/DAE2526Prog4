#include "ShootCommand.h"

#include "Scene.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "BulletComponent.h"
#include "CollisionComponent.h"

#include "ServiceLocator.h"
#include "SoundIds.h"

ShootCommand::ShootCommand(dae::GameObject& shooter, dae::Scene& scene)
	: m_Shooter(shooter)
	, m_Scene(scene)
{
}

void ShootCommand::Execute(float)
{
	auto* shooterTransform = m_Shooter.GetComponent<dae::TransformComponent>();
	if (!shooterTransform)
		return;

	auto position = shooterTransform->GetLocalPosition();

	auto bullet = std::make_unique<dae::GameObject>();
	bullet->AddComponent<dae::TransformComponent>();
	bullet->GetComponent<dae::TransformComponent>()->SetLocalPosition(
		position.x + 2.f,
		position.y - 10.f
	);

	bullet->AddComponent<dae::RenderComponent>("Sprites/BulletSprite.png");
	bullet->AddComponent<CollisionComponent>(4.f, 12.f);
	bullet->AddComponent<BulletComponent>(m_Scene, 400.f);

	m_Scene.Add(std::move(bullet));

	dae::ServiceLocator::GetSoundSystem().Play(galaga::SoundIds::Shoot, 1.0f);
}