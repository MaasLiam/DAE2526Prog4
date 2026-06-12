#pragma once

#include "Component.h"
#include "GameState.h"
#include "HighScoreManager.h"
#include "GameMode.h"

#include <array>
#include <string>
#include <vector>
#include <glm/vec3.hpp>

namespace dae
{
	class GameObject;
	class Scene;
	class TextComponent;
	class TransformComponent;
}

namespace galaga
{
	enum class PlayerIndex
	{
		PlayerOne = 0,
		PlayerTwo = 1
	};

	class GalagaGameControllerComponent final : public dae::Component
	{
	public:
		GalagaGameControllerComponent(
			dae::GameObject* owner,
			dae::Scene& scene,
			dae::TextComponent& titleText,
			dae::TextComponent& scoreText,
			dae::TransformComponent& scoreTextTransform,
			dae::TextComponent& initialsText,
			dae::TransformComponent& initialsTextTransform,
			dae::TextComponent& instructionText,
			dae::TransformComponent& instructionTextTransform,
			dae::TextComponent& tableTitleText,
			dae::TransformComponent& tableTitleTextTransform,
			std::array<dae::TextComponent*, 5> highScoreRows,
			dae::TextComponent& controlsP1Text,
			dae::TextComponent& controlsP2Text
		);

		~GalagaGameControllerComponent() override = default;

		GalagaGameControllerComponent(const GalagaGameControllerComponent&) = delete;
		GalagaGameControllerComponent(GalagaGameControllerComponent&&) = delete;
		GalagaGameControllerComponent& operator=(const GalagaGameControllerComponent&) = delete;
		GalagaGameControllerComponent& operator=(GalagaGameControllerComponent&&) = delete;

		void Update(float deltaTime) override;

		void RegisterPlayer(dae::GameObject* player);
		void RegisterObjectToHideOnResults(dae::GameObject* object);
		void RegisterGameplayObject(dae::GameObject* object, const glm::vec3& gameplayPosition);
		void RegisterPlayerTwoGameplayObject(dae::GameObject* object, const glm::vec3& gameplayPosition);

		galaga::GameState GetState() const;
		void ForceRefreshCurrentState();

		void StartGame();
		void SkipStage();
		void ReturnToModeSelection();

		void ChangeSelectedInitial(int direction);
		void MoveInitialCursor(int direction);
		void ConfirmHighScoreName();

		void MoveMenuSelection(int direction);
		void ConfirmCurrentSelection();

		void RegisterShotFired();
		void RegisterHit();

		void SelectGameMode(galaga::GameMode gameMode);
		galaga::GameMode GetGameMode() const;
		void SetVersusWinner(int playerIndex);

		bool IsPlayerActive(galaga::PlayerIndex playerIndex) const;
		bool IsPlayerAlive(galaga::PlayerIndex playerIndex) const;
		bool CanPlayerAct(galaga::PlayerIndex playerIndex) const;

		dae::GameObject* GetPlayer(galaga::PlayerIndex playerIndex) const;

	private:
		struct GameplayObject
		{
			dae::GameObject* object{};
			glm::vec3 gameplayPosition{};
		};

		bool AreAllEnemiesDefeated() const;
		bool AreAllPlayersDead() const;
		bool IsPlayerOneDead() const;

		int GetTotalScore() const;
		std::string GetInitialsString() const;

		void SetState(galaga::GameState state);
		void StartStage(int stageIndex);
		void EnterHighScoreScreen();
		void SaveHighScore();

		void RefreshNameEntryText();
		void RefreshHighScoreTable();
		void HideResultTexts();
		void HideGameplayInstructionTexts();
		void RefreshModeSelectionText();
		void HideGameplayObjects();
		void ShowGameplayObjects();
		void ShowPlayerTwoObjects();

		dae::Scene& m_Scene;

		dae::TextComponent& m_TitleText;
		dae::TextComponent& m_ScoreText;
		dae::TransformComponent& m_ScoreTextTransform;
		dae::TextComponent& m_InitialsText;
		dae::TransformComponent& m_InitialsTextTransform;
		dae::TextComponent& m_InstructionText;
		dae::TransformComponent& m_InstructionTextTransform;
		dae::TextComponent& m_TableTitleText;
		dae::TransformComponent& m_TableTitleTextTransform;
		std::array<dae::TextComponent*, 5> m_HighScoreRows{};
		dae::TextComponent& m_ControlsP1Text;
		dae::TextComponent& m_ControlsP2Text;

		std::vector<dae::GameObject*> m_Players{};
		std::vector<dae::GameObject*> m_ObjectsToHideOnResults{};
		std::vector<GameplayObject> m_GameplayObjects{};
		std::vector<GameplayObject> m_PlayerTwoGameplayObjects{};

		HighScoreManager m_HighScoreManager{ "highscores.txt" };

		galaga::GameState m_State{ galaga::GameState::StartScreen };
		float m_StateTimer{};
		int m_StageIndex{ 1 };
		int m_FinalScore{};

		int m_ShotsFired{};
		int m_Hits{};

		std::array<char, 3> m_Initials{ 'A', 'A', 'A' };
		int m_SelectedInitialIndex{};

		static constexpr int MaxStageIndex{ 3 };
		galaga::GameMode m_GameMode{ galaga::GameMode::SinglePlayer };
		int m_SelectedGameModeIndex{};
		std::string m_ResultMessage{};
	};
}

