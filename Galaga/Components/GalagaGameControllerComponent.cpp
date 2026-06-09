#include "GalagaGameControllerComponent.h"

#include "EnemyComponent.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "LevelLoader.h"
#include "Scene.h"
#include "TextComponent.h"

#include <string>

GalagaGameControllerComponent::GalagaGameControllerComponent(dae::GameObject* owner, dae::Scene& scene, dae::TextComponent& messageText)
	: dae::Component(owner)
	, m_Scene(scene)
	, m_MessageText(messageText)
{
	SetState(GameState::StartScreen);
}

void GalagaGameControllerComponent::Update(float deltaTime)
{
	m_StateTimer += deltaTime;

	switch (m_State)
	{
	case GameState::StartScreen:
		if (m_StateTimer >= 2.f)
		{
			StartGame();
		}
		break;

	case GameState::Playing:
		if (AreAllPlayersDead())
		{
			SetGameOver();
			return;
		}

		if (AreAllEnemiesDefeated())
		{
			SetState(GameState::StageComplete);
		}
		break;

	case GameState::StageComplete:
		if (m_StateTimer < 2.f)
		{
			return;
		}

		if (m_StageIndex >= m_MaxStageIndex)
		{
			SetGameOver();
			return;
		}

		StartStage(m_StageIndex + 1);
		break;

	case GameState::GameOver:
		break;
	}
}

void GalagaGameControllerComponent::RegisterPlayer(dae::GameObject* player)
{
	if (player)
	{
		m_Players.push_back(player);
	}
}

GameState GalagaGameControllerComponent::GetState() const
{
	return m_State;
}

void GalagaGameControllerComponent::StartGame()
{
	StartStage(1);
}

void GalagaGameControllerComponent::SkipStage()
{
	if (m_State != GameState::Playing && m_State != GameState::StageComplete)
	{
		return;
	}

	if (m_StageIndex >= m_MaxStageIndex)
	{
		SetGameOver();
		return;
	}

	StartStage(m_StageIndex + 1);
}

void GalagaGameControllerComponent::SetGameOver()
{
	SetState(GameState::GameOver);
}

bool GalagaGameControllerComponent::AreAllEnemiesDefeated() const
{
	for (const auto& object : m_Scene.GetObjects())
	{
		const auto* enemy = object->GetComponent<EnemyComponent>();
		if (enemy && !enemy->IsDead())
		{
			return false;
		}
	}

	return true;
}

bool GalagaGameControllerComponent::AreAllPlayersDead() const
{
	if (m_Players.empty())
	{
		return false;
	}

	for (const auto* player : m_Players)
	{
		if (!player)
		{
			continue;
		}

		const auto* health = player->GetComponent<dae::HealthComponent>();
		if (health && !health->IsDead())
		{
			return false;
		}
	}

	return true;
}

void GalagaGameControllerComponent::SetState(GameState state)
{
	m_State = state;
	m_StateTimer = 0.f;

	switch (m_State)
	{
	case GameState::StartScreen:
		m_MessageText.SetText("GALAGA");
		break;

	case GameState::Playing:
		m_MessageText.SetText("Stage " + std::to_string(m_StageIndex));
		break;

	case GameState::StageComplete:
		m_MessageText.SetText("Stage Clear!");
		break;

	case GameState::GameOver:
		m_MessageText.SetText("Game Over");
		break;
	}
}

void GalagaGameControllerComponent::StartStage(int stageIndex)
{
	m_StageIndex = stageIndex;

	LevelLoader::LoadStage(m_Scene, m_StageIndex);

	SetState(GameState::Playing);
}