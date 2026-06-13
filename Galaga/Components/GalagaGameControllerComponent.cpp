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

#include "RenderComponent.h"
#include "ResourceManager.h"

#include <algorithm>
#include <array>
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
		UpdateStartScreenState();
		break;

	case galaga::GameState::Playing:
		UpdatePlayingState();
		break;

	case galaga::GameState::StageComplete:
		UpdateStageCompleteState();
		break;

	case galaga::GameState::ModeSelection:
	case galaga::GameState::EnteringHighScore:
	case galaga::GameState::HighScoreScreen:
		break;
	}
}

void galaga::GalagaGameControllerComponent::RegisterPlayer(dae::GameObject* player)
{
	if (player == nullptr || IsObjectRegistered(m_Players, player))
	{
		return;
	}

	m_Players.emplace_back(player);
}

void galaga::GalagaGameControllerComponent::RegisterObjectToHideOnResults(dae::GameObject* object)
{
	if (object == nullptr || IsObjectRegistered(m_ObjectsToHideOnResults, object))
	{
		return;
	}

	m_ObjectsToHideOnResults.emplace_back(object);
}

void galaga::GalagaGameControllerComponent::RegisterGameplayObject(dae::GameObject* object, const glm::vec3& gameplayPosition)
{
	if (object == nullptr || IsGameplayObjectRegistered(m_GameplayObjects, object))
	{
		return;
	}

	m_GameplayObjects.emplace_back(GameplayObject{ object, gameplayPosition });
}

void galaga::GalagaGameControllerComponent::RegisterPlayerTwoGameplayObject(dae::GameObject* object, const glm::vec3& gameplayPosition)
{
	if (object == nullptr || IsGameplayObjectRegistered(m_PlayerTwoGameplayObjects, object))
	{
		return;
	}

	m_PlayerTwoGameplayObjects.emplace_back(GameplayObject{ object, gameplayPosition });
}

void galaga::GalagaGameControllerComponent::RegisterMenuObject(dae::GameObject* object)
{
	if (object == nullptr || IsGameplayObjectRegistered(m_MenuObjects, object))
	{
		return;
	}

	auto* transform = object->GetComponent<dae::TransformComponent>();

	if (transform == nullptr)
	{
		return;
	}

	m_MenuObjects.emplace_back(GameplayObject{ object, transform->GetLocalPosition() });
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
		if (m_Players[index] == nullptr)
		{
			continue;
		}

		ResetPlayer(*m_Players[index], index);
	}

	StartStage(1);
}

void galaga::GalagaGameControllerComponent::SkipStage()
{
	if (m_State != galaga::GameState::Playing && m_State != galaga::GameState::StageComplete)
	{
		return;
	}

	if (m_StageIndex >= galaga::gameplay::MaxStageIndex)
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
		m_SelectedInitialIndex = InitialCount - 1;
	}
	else if (m_SelectedInitialIndex >= InitialCount)
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
		m_SelectedGameModeIndex = GameModeCount - 1;
	}
	else if (m_SelectedGameModeIndex >= GameModeCount)
	{
		m_SelectedGameModeIndex = 0;
	}

	RefreshModeSelectionText();
}

void galaga::GalagaGameControllerComponent::ConfirmCurrentSelection()
{
	if (m_State == galaga::GameState::ModeSelection)
	{
		SelectGameMode(GetSelectedGameMode());
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

galaga::GameMode galaga::GalagaGameControllerComponent::GetSelectedGameMode() const
{
	constexpr std::array<galaga::GameMode, GameModeCount> gameModes{galaga::GameMode::SinglePlayer, galaga::GameMode::Coop, galaga::GameMode::Versus};
	const auto selectedIndex = static_cast<size_t>(m_SelectedGameModeIndex);

	if (selectedIndex >= gameModes.size())
	{
		return galaga::GameMode::SinglePlayer;
	}

	return gameModes[selectedIndex];
}

void galaga::GalagaGameControllerComponent::SetState(galaga::GameState state)
{
	m_State = state;
	m_StateTimer = 0.f;

	switch (m_State)
	{
	case galaga::GameState::StartScreen:
		EnterStartScreenState();
		break;

	case galaga::GameState::Playing:
		EnterPlayingState();
		break;

	case galaga::GameState::StageComplete:
		EnterStageCompleteState();
		break;

	case galaga::GameState::EnteringHighScore:
		EnterHighScoreEntryState();
		break;

	case galaga::GameState::HighScoreScreen:
		EnterHighScoreScreenState();
		break;

	case galaga::GameState::ModeSelection:
		EnterModeSelectionState();
		break;
	}
}

void galaga::GalagaGameControllerComponent::EnterStartScreenState()
{
	HideResultTexts();
	HideGameplayObjects();
	ShowMenuObjects();

	m_TitleText.SetText("");
}

void galaga::GalagaGameControllerComponent::EnterPlayingState()
{
	HideResultTexts();
	HideMenuObjects();
	ShowGameplayObjects();

	m_ScoreTextTransform.SetLocalPosition(330.f, 125.f, 0.f);
	m_TitleText.SetText("STAGE " + std::to_string(m_StageIndex));
}

void galaga::GalagaGameControllerComponent::EnterStageCompleteState()
{
	HideResultTexts();
	HideMenuObjects();

	m_TitleText.SetText("STAGE CLEAR!");
}

void galaga::GalagaGameControllerComponent::EnterHighScoreEntryState()
{
	HideMenuObjects();
	HideGameplayInstructionTexts();

	m_TitleText.SetText(m_ResultMessage.empty() ? "- RESULTS -" : m_ResultMessage);
	m_ScoreTextTransform.SetLocalPosition(120.f, 125.f, 0.f);
	m_InitialsTextTransform.SetLocalPosition(390.f, 210.f, 0.f);
	m_InstructionTextTransform.SetLocalPosition(95.f, 285.f, 0.f);
	m_TableTitleTextTransform.SetLocalPosition(320.f, 360.f, 0.f);

	const int hitRatio = m_ShotsFired == 0 ? 0 : (m_Hits * 100) / m_ShotsFired;
	m_ScoreText.SetText("SCORE: " + std::to_string(m_FinalScore) + "   SHOTS: " + std::to_string(m_ShotsFired) + "   HITS: " + std::to_string(m_Hits) + "   HIT%: " + std::to_string(hitRatio));
	m_TableTitleText.SetText("ENTER YOUR INITIALS");
	m_InstructionText.SetText("UP/DOWN CHANGE  LEFT/RIGHT SELECT  ENTER/A SAVE");

	RefreshNameEntryText();
}

void galaga::GalagaGameControllerComponent::EnterHighScoreScreenState()
{
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
}

void galaga::GalagaGameControllerComponent::EnterModeSelectionState()
{
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
}

void galaga::GalagaGameControllerComponent::UpdateStartScreenState()
{
	if (m_StateTimer < StartScreenDuration)
	{
		return;
	}

	SetState(galaga::GameState::ModeSelection);
}

void galaga::GalagaGameControllerComponent::UpdatePlayingState()
{
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
}

void galaga::GalagaGameControllerComponent::UpdateStageCompleteState()
{
	if (m_StateTimer < StageCompleteDuration)
	{
		return;
	}

	if (m_StageIndex >= galaga::gameplay::MaxStageIndex)
	{
		EnterHighScoreScreen();
		return;
	}

	StartStage(m_StageIndex + 1);
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
		HideObject(object);
	}

	SetState(galaga::GameState::EnteringHighScore);
}

void galaga::GalagaGameControllerComponent::ResetPlayer(dae::GameObject& player, size_t playerIndex)
{
	auto* health = player.GetComponent<HealthComponent>();
	if (health != nullptr)
	{
		health->Reset(galaga::gameplay::StartingLives);
	}

	auto* score = player.GetComponent<ScoreComponent>();
	if (score != nullptr)
	{
		score->Reset();
	}

	auto* transform = player.GetComponent<dae::TransformComponent>();
	if (transform != nullptr)
	{
		const auto startPosition = playerIndex == 0 ? galaga::gameplay::PlayerOneStartPosition : galaga::gameplay::PlayerTwoStartPosition;
		transform->SetLocalPosition(startPosition);
	}

	auto* versusBoss = player.GetComponent<VersusBossComponent>();
	if (versusBoss != nullptr)
	{
		versusBoss->Reset();
	}
}

void galaga::GalagaGameControllerComponent::SaveHighScore()
{
	m_HighScoreManager.AddScore(GetInitialsString(), m_FinalScore);
}

void galaga::GalagaGameControllerComponent::RefreshNameEntryText()
{
	std::string initials{};

	for (int index{}; index < InitialCount; ++index)
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
	constexpr std::array<const char*, GameModeCount> modeNames
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
	for (const auto& gameplayObject : m_GameplayObjects)
	{
		HideObject(gameplayObject.object);
	}

	for (const auto& gameplayObject : m_PlayerTwoGameplayObjects)
	{
		HideObject(gameplayObject.object);
	}
}

void galaga::GalagaGameControllerComponent::ShowGameplayObjects()
{
	for (const auto& gameplayObject : m_GameplayObjects)
	{
		SetObjectPosition(gameplayObject.object, gameplayObject.gameplayPosition);
	}

	if (m_GameMode != galaga::GameMode::SinglePlayer)
	{
		ShowPlayerTwoObjects();
		return;
	}

	for (const auto& gameplayObject : m_PlayerTwoGameplayObjects)
	{
		HideObject(gameplayObject.object);
	}
}

void galaga::GalagaGameControllerComponent::ShowPlayerTwoObjects()
{
	for (const auto& gameplayObject : m_PlayerTwoGameplayObjects)
	{
		if (gameplayObject.object == nullptr)
		{
			continue;
		}

		const bool isPlayerTwoShip = gameplayObject.gameplayPosition.x == galaga::gameplay::PlayerTwoStartX && gameplayObject.gameplayPosition.y == galaga::gameplay::PlayerStartY;

		if (isPlayerTwoShip && m_GameMode == galaga::GameMode::Versus)
		{
			SetObjectTexture(gameplayObject.object, galaga::gameplay::VersusBossSprite);
			SetObjectPosition(gameplayObject.object, galaga::gameplay::VersusBossStartPosition);
			continue;
		}

		if (isPlayerTwoShip)
		{
			SetObjectTexture(gameplayObject.object, galaga::gameplay::FighterTwoSprite);
		}

		SetObjectPosition(gameplayObject.object, gameplayObject.gameplayPosition);
	}
}

void galaga::GalagaGameControllerComponent::HideMenuObjects()
{
	for (const auto& menuObject : m_MenuObjects)
	{
		HideObject(menuObject.object);
	}
}

void galaga::GalagaGameControllerComponent::ShowMenuObjects()
{
	for (const auto& menuObject : m_MenuObjects)
	{
		SetObjectPosition(menuObject.object, menuObject.gameplayPosition);
	}
}

void galaga::GalagaGameControllerComponent::SetObjectPosition(dae::GameObject* object, const glm::vec3& position) const
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

	transform->SetLocalPosition(position);
}

void galaga::GalagaGameControllerComponent::HideObject(dae::GameObject * object) const
{
	SetObjectPosition(object, glm::vec3{galaga::gameplay::HiddenObjectPosition, galaga::gameplay::HiddenObjectPosition, 0.f});
}

void galaga::GalagaGameControllerComponent::SetObjectTexture(dae::GameObject* object, const char* texturePath) const
{
	if (object == nullptr || texturePath == nullptr)
	{
		return;
	}

	auto* renderComponent = object->GetComponent<dae::RenderComponent>();

	if (renderComponent == nullptr)
	{
		return;
	}

	const auto texture = dae::ResourceManager::GetInstance().LoadTexture(texturePath);
	renderComponent->SetTexture(texture.get());
}

bool galaga::GalagaGameControllerComponent::IsObjectRegistered(const std::vector<dae::GameObject*>& objects, dae::GameObject* object) const
{
	return std::find(objects.begin(), objects.end(), object) != objects.end();
}

bool galaga::GalagaGameControllerComponent::IsGameplayObjectRegistered(const std::vector<GameplayObject>& objects, dae::GameObject* object) const
{
	return std::find_if(
		objects.begin(),
		objects.end(),
		[object](const GameplayObject& gameplayObject)
		{
			return gameplayObject.object == object;
		}
	) != objects.end();
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
