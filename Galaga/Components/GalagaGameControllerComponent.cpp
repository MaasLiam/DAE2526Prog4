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

#include <string>

GalagaGameControllerComponent::GalagaGameControllerComponent(
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
)
	: dae::Component(owner)
	, m_Scene(scene)
	, m_TitleText(titleText)
	, m_ScoreText(scoreText)
	, m_InitialsText(initialsText)
	, m_InstructionText(instructionText)
	, m_TableTitleText(tableTitleText)
	, m_HighScoreRows(highScoreRows)
	, m_ControlsP1Text(controlsP1Text)
	, m_ControlsP2Text(controlsP2Text)
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
			EnterHighScoreScreen();
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
			EnterHighScoreScreen();
			return;
		}

		StartStage(m_StageIndex + 1);
		break;

	case GameState::EnteringHighScore:
	case GameState::HighScoreScreen:
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

void GalagaGameControllerComponent::RegisterObjectToHideOnResults(dae::GameObject* object)
{
	if (object)
	{
		m_ObjectsToHideOnResults.push_back(object);
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
		EnterHighScoreScreen();
		return;
	}

	StartStage(m_StageIndex + 1);
}

void GalagaGameControllerComponent::ChangeSelectedInitial(int direction)
{
	if (m_State != GameState::EnteringHighScore)
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

void GalagaGameControllerComponent::MoveInitialCursor(int direction)
{
	if (m_State != GameState::EnteringHighScore)
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

void GalagaGameControllerComponent::ConfirmHighScoreName()
{
	if (m_State != GameState::EnteringHighScore)
	{
		return;
	}

	SaveHighScore();
	SetState(GameState::HighScoreScreen);
}

void GalagaGameControllerComponent::RegisterShotFired()
{
	++m_ShotsFired;
}

void GalagaGameControllerComponent::RegisterHit()
{
	++m_Hits;
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

int GalagaGameControllerComponent::GetTotalScore() const
{
	int totalScore{};

	for (const auto* player : m_Players)
	{
		if (!player)
		{
			continue;
		}

		const auto* score = player->GetComponent<dae::ScoreComponent>();
		if (score)
		{
			totalScore += score->GetScore();
		}
	}

	return totalScore;
}

std::string GalagaGameControllerComponent::GetInitialsString() const
{
	return std::string{ m_Initials.begin(), m_Initials.end() };
}

void GalagaGameControllerComponent::SetState(GameState state)
{
	m_State = state;
	m_StateTimer = 0.f;

	switch (m_State)
	{
	case GameState::StartScreen:
		HideResultTexts();
		m_TitleText.SetText("GALAGA");
		break;

	case GameState::Playing:
		HideResultTexts();
		m_TitleText.SetText("STAGE " + std::to_string(m_StageIndex));
		break;

	case GameState::StageComplete:
		HideResultTexts();
		m_TitleText.SetText("STAGE CLEAR!");
		break;

	case GameState::EnteringHighScore:
	{
		HideGameplayInstructionTexts();
		m_TitleText.SetText("- RESULTS -");

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

	case GameState::HighScoreScreen:
		m_TitleText.SetText("- HIGHSCORES -");
		m_ScoreText.SetText("FINAL SCORE: " + std::to_string(m_FinalScore));
		m_InitialsText.SetText("");
		m_InstructionText.SetText("SAVED TO highscores.txt");
		m_TableTitleText.SetText("RANK   NAME   SCORE");
		RefreshHighScoreTable();
		break;
	}
}

void GalagaGameControllerComponent::StartStage(int stageIndex)
{
	m_StageIndex = stageIndex;

	LevelLoader::LoadStage(m_Scene, m_StageIndex);

	SetState(GameState::Playing);
}

void GalagaGameControllerComponent::EnterHighScoreScreen()
{
	m_FinalScore = GetTotalScore();

	LevelLoader::ClearStage(m_Scene);

	for (auto* object : m_ObjectsToHideOnResults)
	{
		if (object)
		{
			m_Scene.Remove(*object);
		}
	}

	m_ObjectsToHideOnResults.clear();

	for (const auto& object : m_Scene.GetObjects())
	{
		if (object->GetComponent<dae::HealthComponent>() ||
			object->GetComponent<dae::ScoreComponent>() ||
			object->GetComponent<dae::DisplayLivesComponent>() ||
			object->GetComponent<dae::DisplayScoreComponent>())
		{
			m_Scene.Remove(*object);
		}
	}

	m_Players.clear();

	SetState(GameState::EnteringHighScore);
}

void GalagaGameControllerComponent::SaveHighScore()
{
	m_HighScoreManager.AddScore(GetInitialsString(), m_FinalScore);
}

void GalagaGameControllerComponent::RefreshNameEntryText()
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

void GalagaGameControllerComponent::RefreshHighScoreTable()
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

void GalagaGameControllerComponent::HideResultTexts()
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

void GalagaGameControllerComponent::HideGameplayInstructionTexts()
{
	m_ControlsP1Text.SetText("");
	m_ControlsP2Text.SetText("");
}