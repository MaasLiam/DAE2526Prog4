#include "VersusBossComponent.h"

#include "CollisionComponent.h"
#include "GalagaGameControllerComponent.h"
#include "GameMode.h"
#include "GameObject.h"
#include "GameState.h"
#include "HealthComponent.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "ServiceLocator.h"
#include "SoundIds.h"
#include "GameplayConstants.h"

#include <glm/geometric.hpp>

galaga::VersusBossComponent::VersusBossComponent(dae::GameObject* owner, GalagaGameControllerComponent& gameController, dae::GameObject& targetPlayer)
	: dae::Component(owner)
	, m_GameController(gameController)
	, m_TargetPlayer(targetPlayer)
{

}

void galaga::VersusBossComponent::Update(float deltaTime)
{
	if (m_GameController.GetState() != galaga::GameState::Playing || m_GameController.GetGameMode() != galaga::GameMode::Versus)
	{
		return;
	}

	switch (m_State)
	{
	case BossState::Idle:
		break;

	case BossState::DivingToPlayer:
		MoveTowards(m_DiveTarget, 260.f, deltaTime);
		TryDamagePlayerOnContact();

		if (IsNear(m_DiveTarget, 12.f))
		{
			m_State = BossState::ExitingBottom;
		}
		break;

	case BossState::ExitingBottom:
	{
		auto* transform = GetOwner()->GetComponent<dae::TransformComponent>();
		if (!transform)
		{
			return;
		}

		auto position = transform->GetLocalPosition();
		position.y += 300.f * deltaTime;
		transform->SetLocalPosition(position);

		TryDamagePlayerOnContact();

		if (position.y > 620.f)
		{
			transform->SetLocalPosition(m_StartPosition.x, -80.f, 0.f);
			m_State = BossState::Returning;
		}

		break;
	}

	case BossState::Returning:
		MoveTowards(m_StartPosition, 220.f, deltaTime);

		if (IsNear(m_StartPosition, 8.f))
		{
			auto* transform = GetOwner()->GetComponent<dae::TransformComponent>();
			if (transform)
			{
				transform->SetLocalPosition(m_StartPosition);
			}

			m_State = BossState::Idle;
		}
		break;

	case BossState::MovingToBeamPosition:
		MoveTowards(m_BeamPosition, 180.f, deltaTime);

		if (IsNear(m_BeamPosition, 8.f))
		{
			auto* transform = GetOwner()->GetComponent<dae::TransformComponent>();
			if (transform)
			{
				transform->SetLocalPosition(m_BeamPosition);
			}

			m_TractorTimer = 0.f;
			m_State = BossState::TractorBeam;

			dae::ServiceLocator::GetSoundSystem().Play(galaga::ToSoundId(galaga::SoundIds::TractorBeam), 1.0f);
		}

		break;

	case BossState::TractorBeam:
		SetBeamVisible(true);

		m_TractorTimer += deltaTime;
		TryCapturePlayerWithBeam();

		if (m_TractorTimer >= 2.5f)
		{
			m_TractorTimer = 0.f;
			SetBeamVisible(false);
			m_State = BossState::ReturningFromBeam;
		}

		break;

	case BossState::ReturningFromBeam:
		MoveTowards(m_StartPosition, 180.f, deltaTime);

		if (IsNear(m_StartPosition, 8.f))
		{
			auto* transform = GetOwner()->GetComponent<dae::TransformComponent>();
			if (transform)
			{
				transform->SetLocalPosition(m_StartPosition);
			}

			m_State = BossState::Idle;
		}
		break;
	}
}

void galaga::VersusBossComponent::StartDive()
{
	SetBeamVisible(false);
	if (m_GameController.GetState() != galaga::GameState::Playing || m_GameController.GetGameMode() != galaga::GameMode::Versus || m_State != BossState::Idle)
	{
		return;
	}

	auto* playerTransform = m_TargetPlayer.GetComponent<dae::TransformComponent>();
	if (!playerTransform)
	{
		return;
	}

	const auto playerPosition = playerTransform->GetLocalPosition();
	m_DiveTarget = glm::vec3{ playerPosition.x, playerPosition.y - 20.f, 0.f };
	m_HasDamagedPlayerThisAttack = false;
	m_State = BossState::DivingToPlayer;

	dae::ServiceLocator::GetSoundSystem().Play(galaga::ToSoundId(galaga::SoundIds::EnemyDive), 1.0f);
}

void galaga::VersusBossComponent::StartTractorBeam()
{
	if (m_GameController.GetState() != galaga::GameState::Playing || m_GameController.GetGameMode() != galaga::GameMode::Versus || m_State != BossState::Idle)
	{
		return;
	}

	m_TractorTimer = 0.f;
	m_HasDamagedPlayerThisAttack = false;
	SetBeamVisible(false);
	m_State = BossState::MovingToBeamPosition;

	dae::ServiceLocator::GetSoundSystem().Play(galaga::ToSoundId(galaga::SoundIds::EnemyDive), 1.0f);
}

void galaga::VersusBossComponent::TakeHit()
{
	if (m_HitPoints <= 0)
	{
		return;
	}

	--m_HitPoints;

	if (!m_DamagedVisualApplied)
	{
		auto* render = GetOwner()->GetComponent<dae::RenderComponent>();
		if (render)
		{
			render->SetTexture("Sprites/BossGalagaDamaged.png");
		}

		m_DamagedVisualApplied = true;
	}
}

bool galaga::VersusBossComponent::IsDead() const
{
	return m_HitPoints <= 0;
}

void galaga::VersusBossComponent::SetBeamVisual(dae::GameObject* beamVisual)
{
	m_BeamVisual = beamVisual;
}

void galaga::VersusBossComponent::Reset()
{
	m_State = BossState::Idle;
	m_TractorTimer = 0.f;
	m_HasDamagedPlayerThisAttack = false;
	m_HitPoints = 4;
	m_DamagedVisualApplied = false;

	SetBeamVisible(false);

	auto* transform = GetOwner()->GetComponent<dae::TransformComponent>();
	if (transform)
	{
		transform->SetLocalPosition(m_StartPosition);
	}

	auto* render = GetOwner()->GetComponent<dae::RenderComponent>();
	if (render)
	{
		render->SetTexture("Sprites/BossGalaga.png");
	}
}

void galaga::VersusBossComponent::MoveTowards(const glm::vec3& target, float speed, float deltaTime)
{
	auto* transform = GetOwner()->GetComponent<dae::TransformComponent>();
	if (!transform)
	{
		return;
	}

	auto position = transform->GetLocalPosition();
	const glm::vec3 difference = target - position;
	const float distance = glm::length(difference);

	if (distance <= 0.001f)
	{
		return;
	}

	const glm::vec3 direction = difference / distance;
	const glm::vec3 movement = direction * speed * deltaTime;

	if (glm::length(movement) >= distance)
	{
		transform->SetLocalPosition(target);
		return;
	}

	transform->SetLocalPosition(position + movement);
}

bool galaga::VersusBossComponent::IsNear(const glm::vec3& target, float distance) const
{
	const auto* transform = GetOwner()->GetComponent<dae::TransformComponent>();
	if (!transform)
	{
		return false;
	}

	return glm::length(transform->GetLocalPosition() - target) <= distance;
}

void galaga::VersusBossComponent::TryDamagePlayerOnContact()
{
	if (m_HasDamagedPlayerThisAttack)
	{
		return;
	}

	auto* bossCollision = GetOwner()->GetComponent<CollisionComponent>();
	auto* playerCollision = m_TargetPlayer.GetComponent<CollisionComponent>();
	auto* playerHealth = m_TargetPlayer.GetComponent<HealthComponent>();

	if (!bossCollision || !playerCollision || !playerHealth || playerHealth->IsDead())
	{
		return;
	}

	if (!bossCollision->Overlaps(*playerCollision))
	{
		return;
	}

	playerHealth->LoseLife();
	dae::ServiceLocator::GetSoundSystem().Play(galaga::ToSoundId(galaga::SoundIds::PlayerHit), 1.0f);
	m_HasDamagedPlayerThisAttack = true;
}

void galaga::VersusBossComponent::TryCapturePlayerWithBeam()
{
	if (m_HasDamagedPlayerThisAttack)
	{
		return;
	}

	auto* bossTransform = GetOwner()->GetComponent<dae::TransformComponent>();
	auto* playerTransform = m_TargetPlayer.GetComponent<dae::TransformComponent>();
	auto* playerHealth = m_TargetPlayer.GetComponent<HealthComponent>();

	if (!bossTransform || !playerTransform || !playerHealth || playerHealth->IsDead())
	{
		return;
	}

	const auto bossPosition = bossTransform->GetLocalPosition();
	const auto playerPosition = playerTransform->GetLocalPosition();
	const float beamLeft = bossPosition.x + m_BeamHitboxLeftOffset;
	const float beamRight = beamLeft + m_BeamHitboxWidth;
	const float beamTop = bossPosition.y + m_BeamHitboxTopOffset;
	const float beamBottom = beamTop + m_BeamHitboxHeight;

	const bool playerInsideBeamX = playerPosition.x >= beamLeft && playerPosition.x <= beamRight;

	const bool playerInsideBeamY = playerPosition.y >= beamTop && playerPosition.y <= beamBottom;

	if (m_TractorTimer > 0.5f && playerInsideBeamX && playerInsideBeamY)
	{
		playerHealth->LoseLife();
		dae::ServiceLocator::GetSoundSystem().Play(galaga::ToSoundId(galaga::SoundIds::PlayerHit), 1.0f);
		playerTransform->SetLocalPosition(galaga::gameplay::PlayerOneStartPosition);

		m_HasDamagedPlayerThisAttack = true;
	}
}

void galaga::VersusBossComponent::SetBeamVisible(bool isVisible)
{
	if (!m_BeamVisual)
	{
		return;
	}

	auto* beamTransform = m_BeamVisual->GetComponent<dae::TransformComponent>();
	if (!beamTransform)
	{
		return;
	}

	if (!isVisible)
	{
		beamTransform->SetLocalPosition(-1000.f, -1000.f, 0.f);
		return;
	}

	auto* bossTransform = GetOwner()->GetComponent<dae::TransformComponent>();
	if (!bossTransform)
	{
		return;
	}

	const auto bossPosition = bossTransform->GetLocalPosition();
	beamTransform->SetLocalPosition(bossPosition + m_BeamVisualOffset);
}
