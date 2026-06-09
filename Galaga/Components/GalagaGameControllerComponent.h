#pragma once

#include "Component.h"
#include "GameState.h"

#include <vector>

namespace dae
{
	class GameObject;
	class Scene;
	class TextComponent;
}

class GalagaGameControllerComponent final : public dae::Component
{
public:
	GalagaGameControllerComponent(dae::GameObject* owner, dae::Scene& scene, dae::TextComponent& messageText);

	void Update(float deltaTime) override;

	void RegisterPlayer(dae::GameObject* player);

	GameState GetState() const;
	void StartGame();
	void SkipStage();
	void SetGameOver();

private:
	bool AreAllEnemiesDefeated() const;
	bool AreAllPlayersDead() const;

	void SetState(GameState state);
	void StartStage(int stageIndex);

	dae::Scene& m_Scene;
	dae::TextComponent& m_MessageText;

	std::vector<dae::GameObject*> m_Players{};

	GameState m_State{ GameState::StartScreen };
	float m_StateTimer{};
	int m_StageIndex{ 1 };

	static constexpr int m_MaxStageIndex{ 3 };
};