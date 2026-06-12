#include "BossTractorBeamComponent.h"

#include "CollisionComponent.h"
#include "EnemyComponent.h"
#include "GameMode.h"
#include "GameObject.h"
#include "GameState.h"
#include "HealthComponent.h"
#include "RenderComponent.h"
#include "Scene.h"
#include "ServiceLocator.h"
#include "SoundIds.h"
#include "TractorBeamVisualComponent.h"
#include "TransformComponent.h"
#include "CapturedFighterComponent.h"
#include "GameplayConstants.h"

#include <memory>

galaga::BossTractorBeamComponent::BossTractorBeamComponent(dae::GameObject* owner, dae::Scene& scene, GalagaGameControllerComponent& gameController)
	: dae::Component(owner)
	, m_Scene(scene)
	, m_GameController(gameController)
{

}

void galaga::BossTractorBeamComponent::Update(float)
{
	auto* enemy = GetOwner()->GetComponent<EnemyComponent>();
	if (!enemy || enemy->IsDead() || enemy->GetType() != galaga::EnemyType::BossGalaga)
	{
		RemoveBeamVisual();
		return;
	}

	if (m_GameController.GetState() != galaga::GameState::Playing || m_GameController.GetGameMode() == galaga::GameMode::Versus)
	{
		RemoveBeamVisual();
		return;
	}

	if (!enemy->IsTractorBeamActive())
	{
		RemoveBeamVisual();
		m_HasCapturedThisBeam = false;
		return;
	}

	CreateBeamVisual();
	UpdateBeamVisualPosition();

	if (m_HasCapturedThisBeam)
	{
		return;
	}

	if (TryCapturePlayer(galaga::PlayerIndex::PlayerOne))
	{
		m_HasCapturedThisBeam = true;
		return;
	}

	if (TryCapturePlayer(galaga::PlayerIndex::PlayerTwo))
	{
		m_HasCapturedThisBeam = true;
	}
}

void galaga::BossTractorBeamComponent::CreateBeamVisual()
{
	if (m_BeamVisual)
	{
		return;
	}

	auto beam = std::make_unique<dae::GameObject>();
	beam->AddComponent<dae::TransformComponent>();
	beam->AddComponent<dae::RenderComponent>(galaga::gameplay::TractorBeamSprite);
	beam->AddComponent<TractorBeamVisualComponent>();

	m_BeamVisual = beam.get();
	m_Scene.Add(std::move(beam));
}

void galaga::BossTractorBeamComponent::RemoveBeamVisual()
{
	if (!m_BeamVisual)
	{
		return;
	}

	m_Scene.Remove(*m_BeamVisual);
	m_BeamVisual = nullptr;
}

void galaga::BossTractorBeamComponent::UpdateBeamVisualPosition()
{
	if (!m_BeamVisual)
	{
		return;
	}

	auto* bossTransform = GetOwner()->GetComponent<dae::TransformComponent>();
	auto* beamTransform = m_BeamVisual->GetComponent<dae::TransformComponent>();

	if (!bossTransform || !beamTransform)
	{
		return;
	}

	beamTransform->SetLocalPosition(bossTransform->GetLocalPosition() + BeamVisualOffset);
}

bool galaga::BossTractorBeamComponent::TryCapturePlayer(galaga::PlayerIndex playerIndex)
{
	if (!m_GameController.IsPlayerActive(playerIndex) || !m_GameController.IsPlayerAlive(playerIndex))
	{
		return false;
	}

	auto* player = m_GameController.GetPlayer(playerIndex);
	if (!player)
	{
		return false;
	}

	auto* bossTransform = GetOwner()->GetComponent<dae::TransformComponent>();
	auto* playerTransform = player->GetComponent<dae::TransformComponent>();
	auto* playerHealth = player->GetComponent<HealthComponent>();

	if (!bossTransform || !playerTransform || !playerHealth)
	{
		return false;
	}

	const auto bossPosition = bossTransform->GetLocalPosition();
	const auto playerPosition = playerTransform->GetLocalPosition();

	const float beamLeft = bossPosition.x + BeamHitboxLeftOffset;
	const float beamRight = beamLeft + BeamHitboxWidth;
	const float beamTop = bossPosition.y + BeamHitboxTopOffset;
	const float beamBottom = beamTop + BeamHitboxHeight;

	const bool playerInsideBeamX = playerPosition.x >= beamLeft && playerPosition.x <= beamRight;
	const bool playerInsideBeamY = playerPosition.y >= beamTop && playerPosition.y <= beamBottom;

	if (!playerInsideBeamX || !playerInsideBeamY)
	{
		return false;
	}

	playerHealth->LoseLife();
	dae::ServiceLocator::GetSoundSystem().Play(galaga::ToSoundId(galaga::SoundIds::PlayerHit), 1.0f);

	SpawnCapturedFighter(playerIndex);

	const glm::vec3 resetPosition = playerIndex == galaga::PlayerIndex::PlayerOne ? galaga::gameplay::PlayerOneStartPosition : galaga::gameplay::PlayerTwoStartPosition;
	playerTransform->SetLocalPosition(resetPosition);

	return true;
}

void galaga::BossTractorBeamComponent::SpawnCapturedFighter(galaga::PlayerIndex playerIndex)
{
	auto* bossTransform = GetOwner()->GetComponent<dae::TransformComponent>();
	if (!bossTransform)
	{
		return;
	}

	auto* enemy = GetOwner()->GetComponent<EnemyComponent>();
	if (!enemy)
	{
		return;
	}

	const auto formationPosition = enemy->GetFormationPosition();
	auto capturedFighter = std::make_unique<dae::GameObject>();
	capturedFighter->AddComponent<dae::TransformComponent>();
	const float xOffset = playerIndex == galaga::PlayerIndex::PlayerOne ? galaga::gameplay::CapturedFighterPlayerOneOffsetX : galaga::gameplay::CapturedFighterPlayerTwoOffsetX;
	capturedFighter->GetComponent<dae::TransformComponent>()->SetLocalPosition(formationPosition.x + xOffset, formationPosition.y + galaga::gameplay::CapturedFighterOffsetY, 0.f);

	capturedFighter->AddComponent<dae::RenderComponent>(galaga::gameplay::CapturedFighterSprite);
	capturedFighter->AddComponent<CollisionComponent>(galaga::gameplay::FighterCollisionWidth, galaga::gameplay::FighterCollisionHeight);

	capturedFighter->AddComponent<CapturedFighterComponent>(m_Scene, m_GameController);

	m_Scene.Add(std::move(capturedFighter));
}
