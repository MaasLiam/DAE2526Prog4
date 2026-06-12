#include "GalagaGameControllerComponent.h"

#include "BulletComponent.h"
#include "DisplayLivesComponent.h"
#include "DisplayScoreComponent.h"
#include "EnemyComponent.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "LevelLoader.h"
#include "Scene.h"
#include "ScoreComponent.h"
#include "TextComponent.h"
#include "TransformComponent.h"
#include "VersusBossComponent.h"
#include "GameplayConstants.h"

#include "ServiceLocator.h"
#include "SoundIds.h"

#include <string>

galaga::GalagaGameControllerComponent::GalagaGameControllerComponent(dae::GameObject* owner, dae::Scene& scene, GameControllerUi ui)
	: Component(owner)
	, m_Scene(scene)
	, m_TitleText(ui.titleText)
	, m_ScoreText(ui.scoreText)
	, m_ScoreTextTransform(ui.scoreTextTransform)
	, m_InitialsText(ui.initialsText)
	, m_InitialsTextTransform(ui.initialsTextTransform)
	, m_InstructionText(ui.instructionText)
	, m_InstructionTextTransform(ui.instructionTextTransform)
	, m_TableTitleText(ui.tableTitleText)
	, m_TableTitleTextTransform(ui.tableTitleTextTransform)
	, m_HighScoreRows(ui.highScoreRows)
	, m_ControlsP1Text(ui.controlsP1Text)
	, m_ControlsP2Text(ui.controlsP2Text)
{
	ForceRefreshCurrentState();
}

void galaga::GalagaGameControllerComponent::Update(float deltaTime)
{
	m_StateTimer += deltaTime;

	switch (m_State)
	{
	case galaga::GameState::StartScreen:
		if (m_StateTimer >= 1.5f)
		{
			SetState(galaga::GameState::ModeSelection);
		}
		break;

	case galaga::GameState::Playing:
		if (m_GameMode == galaga::GameMode::Versus)
		{
			if (IsPlayerOneDead())
			{
				SetVersusWinner(2);
			}

			return;
		}

		if (AreAllPlayersDead())
		{
			EnterHighScoreScreen();
			return;
		}

		if (AreAllEnemiesDefeated())
		{
			SetState(galaga::GameState::StageComplete);
		}
		break;

	case galaga::GameState::StageComplete:
		if (m_StateTimer < 2.f)
		{
			return;
		}

		if (m_StageIndex >= MaxStageIndex)
		{
			EnterHighScoreScreen();
			return;
		}

		StartStage(m_StageIndex + 1);
		break;
	case galaga::GameState::ModeSelection:
		break;
	case galaga::GameState::EnteringHighScore:
	case galaga::GameState::HighScoreScreen:
		break;
	}
}

void galaga::GalagaGameControllerComponent::RegisterPlayer(dae::GameObject* player)
{
	if (player)
	{
		m_Players.push_back(player);
	}
}

void galaga::GalagaGameControllerComponent::RegisterObjectToHideOnResults(dae::GameObject* object)
{
	if (object)
	{
		m_ObjectsToHideOnResults.push_back(object);
	}
}

void galaga::GalagaGameControllerComponent::RegisterGameplayObject(dae::GameObject* object, const glm::vec3& gameplayPosition)
{
	if (object)
	{
		m_GameplayObjects.push_back(GameplayObject{ object, gameplayPosition });
	}
}

void galaga::GalagaGameControllerComponent::RegisterPlayerTwoGameplayObject(dae::GameObject* object, const glm::vec3& gameplayPosition)
{
	if (object)
	{
		m_PlayerTwoGameplayObjects.push_back(GameplayObject{ object, gameplayPosition });
	}
}

void galaga::GalagaGameControllerComponent::RegisterMenuObject(dae::GameObject* object)
{
	if (object == nullptr)
	{
		return;
	}

	auto* transform = object->GetComponent<dae::TransformComponent>();

	if (transform == nullptr)
	{
		return;
	}

	m_MenuObjects.push_back(GameplayObject{ object, transform->GetLocalPosition() });
}

galaga::GameState galaga::GalagaGameControllerComponent::GetState() const
{
	return m_State;
}

void galaga::GalagaGameControllerComponent::ForceRefreshCurrentState()
{
	SetState(m_State);
}

void galaga::GalagaGameControllerComponent::StartGame()
{
	m_ResultMessage.clear();
	m_FinalScore = 0;
	m_ShotsFired = 0;
	m_Hits = 0;
	m_StageIndex = 1;

	for (size_t index{}; index < m_Players.size(); ++index)
	{
		auto* player = m_Players[index];
		if (!player)
		{
			continue;
		}

		auto* health = player->GetComponent<HealthComponent>();
		if (health)
		{
			health->Reset(4);
		}

		auto* score = player->GetComponent<ScoreComponent>();
		if (score)
		{
			score->Reset();
		}

		auto* transform = player->GetComponent<dae::TransformComponent>();
		if (transform)
		{
			const glm::vec3 startPosition = index == 0 ? galaga::gameplay::PlayerOneStartPosition : galaga::gameplay::PlayerTwoStartPosition;
			transform->SetLocalPosition(startPosition);
		}

		auto* versusBoss = player->GetComponent<VersusBossComponent>();
		if (versusBoss)
		{
			versusBoss->Reset();
		}
	}

	StartStage(1);
}

void galaga::GalagaGameControllerComponent::SkipStage()
{
	if (m_State != galaga::GameState::Playing && m_State != galaga::GameState::StageComplete)
	{
		return;
	}

	if (m_StageIndex >= MaxStageIndex)
	{
		EnterHighScoreScreen();
		return;
	}

	StartStage(m_StageIndex + 1);
}

void galaga::GalagaGameControllerComponent::ReturnToModeSelection()
{
	m_ResultMessage.clear();
	m_SelectedGameModeIndex = 0;
	m_SelectedInitialIndex = 0;
	m_FinalScore = 0;
	m_ShotsFired = 0;
	m_Hits = 0;
	m_StageIndex = 1;
	m_StateTimer = 0.f;

	for (auto& initial : m_Initials)
	{
		initial = 'A';
	}

	LevelLoader::ClearStage(m_Scene);
	HideGameplayObjects();
	SetState(galaga::GameState::ModeSelection);
}

void galaga::GalagaGameControllerComponent::ChangeSelectedInitial(int direction)
{
	if (m_State == galaga::GameState::ModeSelection)
	{
		MoveMenuSelection(-direction);
		return;
	}

	if (m_State != galaga::GameState::EnteringHighScore)
	{
		return;
	}

	auto& letter = m_Initials[static_cast<size_t>(m_SelectedInitialIndex)];
	letter = static_cast<char>(letter + direction);

	if (letter > 'Z')
	{
		letter = 'A';
	}
	else if (letter < 'A')
	{
		letter = 'Z';
	}

	RefreshNameEntryText();
}

void galaga::GalagaGameControllerComponent::MoveInitialCursor(int direction)
{
	if (m_State != galaga::GameState::EnteringHighScore)
	{
		return;
	}

	m_SelectedInitialIndex += direction;

	if (m_SelectedInitialIndex < 0)
	{
		m_SelectedInitialIndex = 2;
	}
	else if (m_SelectedInitialIndex > 2)
	{
		m_SelectedInitialIndex = 0;
	}

	RefreshNameEntryText();
}

void galaga::GalagaGameControllerComponent::ConfirmHighScoreName()
{
	if (m_State != galaga::GameState::EnteringHighScore)
	{
		return;
	}

	SaveHighScore();
	SetState(galaga::GameState::HighScoreScreen);
}

void galaga::GalagaGameControllerComponent::MoveMenuSelection(int direction)
{
	if (m_State != galaga::GameState::ModeSelection)
	{
		return;
	}

	m_SelectedGameModeIndex += direction;

	if (m_SelectedGameModeIndex < 0)
	{
		m_SelectedGameModeIndex = 2;
	}
	else if (m_SelectedGameModeIndex > 2)
	{
		m_SelectedGameModeIndex = 0;
	}

	RefreshModeSelectionText();
}

void galaga::GalagaGameControllerComponent::ConfirmCurrentSelection()
{
	if (m_State == galaga::GameState::ModeSelection)
	{
		switch (m_SelectedGameModeIndex)
		{
		case 0:
			SelectGameMode(galaga::GameMode::SinglePlayer);
			break;

		case 1:
			SelectGameMode(galaga::GameMode::Coop);
			break;

		case 2:
			SelectGameMode(galaga::GameMode::Versus);
			break;
		}

		return;
	}

	if (m_State == galaga::GameState::EnteringHighScore)
	{
		ConfirmHighScoreName();
		return;
	}

	if (m_State == galaga::GameState::HighScoreScreen)
	{
		ReturnToModeSelection();
	}
}

void galaga::GalagaGameControllerComponent::RegisterShotFired()
{
	++m_ShotsFired;
}

void galaga::GalagaGameControllerComponent::RegisterHit()
{
	++m_Hits;
}

bool galaga::GalagaGameControllerComponent::AreAllEnemiesDefeated() const
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

bool galaga::GalagaGameControllerComponent::AreAllPlayersDead() const
{
	if (m_Players.empty())
	{
		return false;
	}

	if (m_GameMode == galaga::GameMode::SinglePlayer || m_GameMode == galaga::GameMode::Versus)
	{
		return IsPlayerOneDead();
	}

	for (const auto* player : m_Players)
	{
		if (!player)
		{
			continue;
		}

		const auto* health = player->GetComponent<HealthComponent>();
		if (health && !health->IsDead())
		{
			return false;
		}
	}

	return true;
}

bool galaga::GalagaGameControllerComponent::IsPlayerOneDead() const
{
	if (m_Players.empty() || !m_Players[0])
	{
		return false;
	}

	const auto* health = m_Players[0]->GetComponent<HealthComponent>();
	return health && health->IsDead();
}

int galaga::GalagaGameControllerComponent::GetTotalScore() const
{
	int totalScore{};

	for (const auto* player : m_Players)
	{
		if (!player)
		{
			continue;
		}

		const auto* score = player->GetComponent<ScoreComponent>();
		if (score)
		{
			totalScore += score->GetScore();
		}
	}

	return totalScore;
}

std::string galaga::GalagaGameControllerComponent::GetInitialsString() const
{
	return std::string{ m_Initials.begin(), m_Initials.end() };
}

void galaga::GalagaGameControllerComponent::SetState(galaga::GameState state)
{
	m_State = state;
	m_StateTimer = 0.f;

	switch (m_State)
	{
	case galaga::GameState::StartScreen:
		HideResultTexts();
		HideGameplayObjects();
		ShowMenuObjects();
		m_TitleText.SetText("");
		break;
	break;

	case galaga::GameState::Playing:
		HideResultTexts();
		HideMenuObjects();
		ShowGameplayObjects();

		m_ScoreTextTransform.SetLocalPosition(330.f, 125.f, 0.f);
		m_TitleText.SetText("STAGE " + std::to_string(m_StageIndex));
		break;

	case galaga::GameState::StageComplete:
		HideResultTexts();
		HideMenuObjects();
		m_TitleText.SetText("STAGE CLEAR!");
		break;

	case galaga::GameState::EnteringHighScore:
	{
		HideMenuObjects();
		HideGameplayInstructionTexts();
		m_TitleText.SetText(m_ResultMessage.empty() ? "- RESULTS -" : m_ResultMessage);

		m_ScoreTextTransform.SetLocalPosition(120.f, 125.f, 0.f);
		m_InitialsTextTransform.SetLocalPosition(390.f, 210.f, 0.f);
		m_InstructionTextTransform.SetLocalPosition(95.f, 285.f, 0.f);
		m_TableTitleTextTransform.SetLocalPosition(320.f, 360.f, 0.f);

		const int ratio = m_ShotsFired == 0 ? 0 : (m_Hits * 100) / m_ShotsFired;

		m_ScoreText.SetText(
			"SCORE: " + std::to_string(m_FinalScore) +
			"   SHOTS: " + std::to_string(m_ShotsFired) +
			"   HITS: " + std::to_string(m_Hits) +
			"   HIT%: " + std::to_string(ratio)
		);

		m_TableTitleText.SetText("ENTER YOUR INITIALS");
		m_InstructionText.SetText("UP/DOWN CHANGE  LEFT/RIGHT SELECT  ENTER/A SAVE");
		RefreshNameEntryText();
		break;
	}

	case galaga::GameState::HighScoreScreen:
		HideMenuObjects();
		m_ScoreTextTransform.SetLocalPosition(300.f, 125.f, 0.f);
		m_InstructionTextTransform.SetLocalPosition(130.f, 300.f, 0.f);
		m_TableTitleTextTransform.SetLocalPosition(320.f, 360.f, 0.f);

		m_TitleText.SetText("- HIGHSCORES -");
		m_ScoreText.SetText("FINAL SCORE: " + std::to_string(m_FinalScore));
		m_InitialsText.SetText("");
		m_InstructionText.SetText("C/X/A/ENTER RETURN TO MENU");
		m_TableTitleText.SetText("RANK   NAME   SCORE");
		RefreshHighScoreTable();
		break;

	case galaga::GameState::ModeSelection:
		HideResultTexts();
		HideGameplayObjects();
		ShowMenuObjects();

		m_TitleText.SetText("");
		m_ScoreTextTransform.SetLocalPosition(390.f, 235.f, 0.f);
		m_InitialsTextTransform.SetLocalPosition(390.f, 280.f, 0.f);
		m_TableTitleTextTransform.SetLocalPosition(390.f, 325.f, 0.f);
		m_InstructionTextTransform.SetLocalPosition(235.f, 390.f, 0.f);
		m_InstructionText.SetText("UP/DOWN SELECT     C/X/A START");

		RefreshModeSelectionText();
		break;
	}

}

void galaga::GalagaGameControllerComponent::StartStage(int stageIndex)
{
	m_StageIndex = stageIndex;
	dae::ServiceLocator::GetSoundSystem().Play(galaga::ToSoundId(galaga::SoundIds::StageStart), 1.0f);
	galaga::LevelLoader::LoadStage(m_Scene, m_StageIndex, *this);
	SetState(galaga::GameState::Playing);
}

void galaga::GalagaGameControllerComponent::EnterHighScoreScreen()
{
	m_FinalScore = GetTotalScore();

	galaga::LevelLoader::ClearStage(m_Scene);
	HideGameplayObjects();

	for (auto* object : m_ObjectsToHideOnResults)
	{
		if (!object)
		{
			continue;
		}

		auto* transform = object->GetComponent<dae::TransformComponent>();
		if (transform)
		{
			transform->SetLocalPosition(-1000.f, -1000.f, 0.f);
		}
	}

	SetState(galaga::GameState::EnteringHighScore);
}

void galaga::GalagaGameControllerComponent::SaveHighScore()
{
	m_HighScoreManager.AddScore(GetInitialsString(), m_FinalScore);
}

void galaga::GalagaGameControllerComponent::RefreshNameEntryText()
{
	std::string initials{};

	for (int index{}; index < 3; ++index)
	{
		if (index == m_SelectedInitialIndex)
		{
			initials += '[';
			initials += m_Initials[static_cast<size_t>(index)];
			initials += ']';
		}
		else
		{
			initials += ' ';
			initials += m_Initials[static_cast<size_t>(index)];
			initials += ' ';
		}

		initials += ' ';
	}

	m_InitialsText.SetText(initials);
}

void galaga::GalagaGameControllerComponent::RefreshHighScoreTable()
{
	const auto highScores = m_HighScoreManager.GetHighScores();

	for (auto* row : m_HighScoreRows)
	{
		if (row)
		{
			row->SetText("");
		}
	}

	for (size_t index{}; index < highScores.size() && index < m_HighScoreRows.size(); ++index)
	{
		const auto& entry = highScores[index];

		std::string row{};
		row += std::to_string(index + 1);
		row += ".      ";
		row += entry.name;
		row += "      ";
		row += std::to_string(entry.score);

		if (m_HighScoreRows[index])
		{
			m_HighScoreRows[index]->SetText(row);
		}
	}
}

void galaga::GalagaGameControllerComponent::HideResultTexts()
{
	m_ScoreText.SetText("");
	m_InitialsText.SetText("");
	m_InstructionText.SetText("");
	m_TableTitleText.SetText("");

	for (auto* row : m_HighScoreRows)
	{
		if (row)
		{
			row->SetText("");
		}
	}
}

void galaga::GalagaGameControllerComponent::HideGameplayInstructionTexts()
{
	m_ControlsP1Text.SetText("");
	m_ControlsP2Text.SetText("");
}

void galaga::GalagaGameControllerComponent::RefreshModeSelectionText()
{
	constexpr std::array<const char*, 3> modeNames
	{
		"1 PLAYER",
		"2 PLAYERS",
		"VERSUS"
	};

	auto setMenuLine =
		[this, &modeNames](int index, dae::TextComponent& textComponent)
		{
			std::string text{};

			if (index == m_SelectedGameModeIndex)
			{
				text += "> ";
				text += modeNames[static_cast<size_t>(index)];
				textComponent.SetColor(SDL_Color{ 255, 255, 0, 255 });
			}
			else
			{
				text += "  ";
				text += modeNames[static_cast<size_t>(index)];
				textComponent.SetColor(SDL_Color{ 255, 255, 255, 255 });
			}

			textComponent.SetText(text);
		};

	setMenuLine(0, m_ScoreText);
	setMenuLine(1, m_InitialsText);
	setMenuLine(2, m_TableTitleText);
}

void galaga::GalagaGameControllerComponent::HideGameplayObjects()
{
	auto hideObject = [](const GameplayObject& gameplayObject)
		{
			if (!gameplayObject.object)
			{
				return;
			}

			auto* transform = gameplayObject.object->GetComponent<dae::TransformComponent>();
			if (transform)
			{
				transform->SetLocalPosition(-1000.f, -1000.f, 0.f);
			}
		};

	for (const auto& gameplayObject : m_GameplayObjects)
	{
		hideObject(gameplayObject);
	}

	for (const auto& gameplayObject : m_PlayerTwoGameplayObjects)
	{
		hideObject(gameplayObject);
	}
}


void galaga::GalagaGameControllerComponent::ShowGameplayObjects()
{
	auto showObject = [](const GameplayObject& gameplayObject)
		{
			if (!gameplayObject.object)
			{
				return;
			}

			auto* transform = gameplayObject.object->GetComponent<dae::TransformComponent>();
			if (transform)
			{
				transform->SetLocalPosition(gameplayObject.gameplayPosition);
			}
		};

	for (const auto& gameplayObject : m_GameplayObjects)
	{
		showObject(gameplayObject);
	}

	if (m_GameMode != galaga::GameMode::SinglePlayer)
	{
		ShowPlayerTwoObjects();
	}
	else
	{
		for (const auto& gameplayObject : m_PlayerTwoGameplayObjects)
		{
			if (!gameplayObject.object)
			{
				continue;
			}

			auto* transform = gameplayObject.object->GetComponent<dae::TransformComponent>();
			if (transform)
			{
				transform->SetLocalPosition(-1000.f, -1000.f, 0.f);
			}
		}
	}
}

void galaga::GalagaGameControllerComponent::ShowPlayerTwoObjects()
{
	for (const auto& gameplayObject : m_PlayerTwoGameplayObjects)
	{
		if (!gameplayObject.object)
		{
			continue;
		}

		auto* transform = gameplayObject.object->GetComponent<dae::TransformComponent>();
		if (!transform)
		{
			continue;
		}

		if (m_GameMode == galaga::GameMode::Versus)
		{
			const bool isPlayerTwoShip = gameplayObject.gameplayPosition.x == galaga::gameplay::PlayerTwoStartX && gameplayObject.gameplayPosition.y == galaga::gameplay::PlayerStartY;

			if (isPlayerTwoShip)
			{
				transform->SetLocalPosition(galaga::gameplay::VersusBossStartPosition);
			}
			else
			{
				transform->SetLocalPosition(gameplayObject.gameplayPosition);
			}
		}
		else
		{
			transform->SetLocalPosition(gameplayObject.gameplayPosition);
		}
	}
}

void galaga::GalagaGameControllerComponent::HideMenuObjects()
{
	for (const auto& menuObject : m_MenuObjects)
	{
		if (menuObject.object == nullptr)
		{
			continue;
		}

		auto* transform = menuObject.object->GetComponent<dae::TransformComponent>();

		if (transform == nullptr)
		{
			continue;
		}

		transform->SetLocalPosition(-1000.f, -1000.f, 0.f);
	}
}

void galaga::GalagaGameControllerComponent::ShowMenuObjects()
{
	for (const auto& menuObject : m_MenuObjects)
	{
		if (menuObject.object == nullptr)
		{
			continue;
		}

		auto* transform = menuObject.object->GetComponent<dae::TransformComponent>();

		if (transform == nullptr)
		{
			continue;
		}

		transform->SetLocalPosition(menuObject.gameplayPosition);
	}
}

void galaga::GalagaGameControllerComponent::SelectGameMode(galaga::GameMode gameMode)
{
	if (m_State != galaga::GameState::ModeSelection)
	{
		return;
	}

	m_GameMode = gameMode;
	StartGame();
}

galaga::GameMode galaga::GalagaGameControllerComponent::GetGameMode() const
{
	return m_GameMode;
}

void galaga::GalagaGameControllerComponent::SetVersusWinner(int playerIndex)
{
	if (m_State != galaga::GameState::Playing)
	{
		return;
	}

	m_ResultMessage = playerIndex == 1 ? "P1 WINS" : "P2 WINS";
	EnterHighScoreScreen();
}

bool galaga::GalagaGameControllerComponent::IsPlayerActive(galaga::PlayerIndex playerIndex) const
{
	switch (m_GameMode)
	{
	case galaga::GameMode::SinglePlayer:
		return playerIndex == galaga::PlayerIndex::PlayerOne;

	case galaga::GameMode::Coop:
		return playerIndex == galaga::PlayerIndex::PlayerOne || playerIndex == galaga::PlayerIndex::PlayerTwo;

	case galaga::GameMode::Versus:
		return playerIndex == galaga::PlayerIndex::PlayerOne;
	}

	return false;
}

bool galaga::GalagaGameControllerComponent::IsPlayerAlive(galaga::PlayerIndex playerIndex) const
{
	const auto index = static_cast<size_t>(playerIndex);

	if (index >= m_Players.size() || !m_Players[index])
	{
		return false;
	}

	const auto* health = m_Players[index]->GetComponent<HealthComponent>();
	return health && !health->IsDead();
}

bool galaga::GalagaGameControllerComponent::CanPlayerAct(galaga::PlayerIndex playerIndex) const
{
	return m_State == galaga::GameState::Playing && IsPlayerActive(playerIndex) && IsPlayerAlive(playerIndex);
}

void galaga::GalagaGameControllerComponent::RespawnPlayer(galaga::PlayerIndex playerIndex)
{
	const auto index = static_cast<size_t>(playerIndex);

	if (index >= m_Players.size() || m_Players[index] == nullptr)
	{
		return;
	}

	if (!IsPlayerActive(playerIndex) || !IsPlayerAlive(playerIndex))
	{
		return;
	}

	auto* transform = m_Players[index]->GetComponent<dae::TransformComponent>();

	if (transform == nullptr)
	{
		return;
	}

	const auto startPosition = playerIndex == galaga::PlayerIndex::PlayerOne ? galaga::gameplay::PlayerOneStartPosition : galaga::gameplay::PlayerTwoStartPosition;

	if (m_GameMode == galaga::GameMode::Versus && playerIndex == galaga::PlayerIndex::PlayerTwo)
	{
		transform->SetLocalPosition(galaga::gameplay::VersusBossStartPosition);
		return;
	}

	transform->SetLocalPosition(startPosition);
}

dae::GameObject* galaga::GalagaGameControllerComponent::GetPlayer(galaga::PlayerIndex playerIndex) const
{
	const auto index = static_cast<size_t>(playerIndex);

	if (index >= m_Players.size())
	{
		return nullptr;
	}

	return m_Players[index];
}
