#pragma once

#include "Component.h"
#include "GameState.h"
#include "HighScoreManager.h"

#include <array>
#include <string>
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
	GalagaGameControllerComponent(
		dae::GameObject* owner,
		dae::Scene& scene,
		dae::TextComponent& titleText,
		dae::TextComponent& scoreText,
		dae::TextComponent& initialsText,
		dae::TextComponent& instructionText,
		dae::TextComponent& tableTitleText,
		std::array<dae::TextComponent*, 5> highScoreRows,
		dae::TextComponent& controlsP1Text,
		dae::TextComponent& controlsP2Text
	);

	void Update(float deltaTime) override;

	void RegisterPlayer(dae::GameObject* player);
	void RegisterObjectToHideOnResults(dae::GameObject* object);

	GameState GetState() const;

	void StartGame();
	void SkipStage();

	void ChangeSelectedInitial(int direction);
	void MoveInitialCursor(int direction);
	void ConfirmHighScoreName();

	void RegisterShotFired();
	void RegisterHit();

private:
	bool AreAllEnemiesDefeated() const;
	bool AreAllPlayersDead() const;

	int GetTotalScore() const;
	std::string GetInitialsString() const;

	void SetState(GameState state);
	void StartStage(int stageIndex);
	void EnterHighScoreScreen();
	void SaveHighScore();

	void RefreshNameEntryText();
	void RefreshHighScoreTable();
	void HideResultTexts();
	void HideGameplayInstructionTexts();

	dae::Scene& m_Scene;

	dae::TextComponent& m_TitleText;
	dae::TextComponent& m_ScoreText;
	dae::TextComponent& m_InitialsText;
	dae::TextComponent& m_InstructionText;
	dae::TextComponent& m_TableTitleText;
	std::array<dae::TextComponent*, 5> m_HighScoreRows{};
	dae::TextComponent& m_ControlsP1Text;
	dae::TextComponent& m_ControlsP2Text;

	std::vector<dae::GameObject*> m_Players{};
	std::vector<dae::GameObject*> m_ObjectsToHideOnResults{};

	HighScoreManager m_HighScoreManager{ "highscores.txt" };

	GameState m_State{ GameState::StartScreen };
	float m_StateTimer{};
	int m_StageIndex{ 1 };
	int m_FinalScore{};

	int m_ShotsFired{};
	int m_Hits{};

	std::array<char, 3> m_Initials{ 'A', 'A', 'A' };
	int m_SelectedInitialIndex{};

	static constexpr int m_MaxStageIndex{ 3 };
};