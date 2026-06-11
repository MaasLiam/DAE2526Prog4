#include "VersusBossComponent.h"

#include "GalagaGameControllerComponent.h"
#include "GameMode.h"
#include "GameObject.h"
#include "GameState.h"
#include "TransformComponent.h"

VersusBossComponent::VersusBossComponent(dae::GameObject* owner, GalagaGameControllerComponent& gameController)
	: dae::Component(owner)
	, m_GameController(gameController)
{

}

void VersusBossComponent::Update(float deltaTime)
{
	if (m_GameController.GetState() != GameState::Playing || m_GameController.GetGameMode() != GameMode::Versus)
	{
		return;
	}

	auto* transform = GetOwner()->GetComponent<dae::TransformComponent>();
	if (!transform)
	{
		return;
	}

	auto position = transform->GetLocalPosition();

	switch (m_State)
	{
	case BossState::Idle:
		break;

	case BossState::Diving:
		position.y += 220.f * deltaTime;

		if (position.y > 500.f)
		{
			m_State = BossState::Returning;
		}

		transform->SetLocalPosition(position);
		break;

	case BossState::Returning:
		position.y -= 220.f * deltaTime;

		if (position.y <= m_StartPosition.y)
		{
			position = m_StartPosition;
			m_State = BossState::Idle;
		}

		transform->SetLocalPosition(position);
		break;

	case BossState::TractorBeam:
		m_TractorTimer += deltaTime;

		position.y += 45.f * deltaTime;

		if (m_TractorTimer >= 2.5f)
		{
			m_TractorTimer = 0.f;
			position = m_StartPosition;
			m_State = BossState::Idle;
		}

		transform->SetLocalPosition(position);
		break;
	}
}

void VersusBossComponent::StartDive()
{
	if (m_GameController.GetState() != GameState::Playing || m_GameController.GetGameMode() != GameMode::Versus || m_State != BossState::Idle)
	{
		return;
	}

	m_State = BossState::Diving;
}

void VersusBossComponent::StartTractorBeam()
{
	if (m_GameController.GetState() != GameState::Playing || m_GameController.GetGameMode() != GameMode::Versus || m_State != BossState::Idle)
	{
		return;
	}

	m_TractorTimer = 0.f;
	m_State = BossState::TractorBeam;
}