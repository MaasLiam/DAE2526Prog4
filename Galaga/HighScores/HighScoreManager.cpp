#include "HighScoreManager.h"

#include <algorithm>
#include <fstream>

HighScoreManager::HighScoreManager(std::string filePath)
	: m_FilePath(std::move(filePath))
{
	Load();
}

void HighScoreManager::AddScore(const std::string& name, int score)
{
	m_HighScores.push_back(HighScoreEntry{ name, score });
	SortAndTrim();
	Save();
}

std::vector<HighScoreEntry> HighScoreManager::GetHighScores() const
{
	return m_HighScores;
}

void HighScoreManager::Load()
{
	m_HighScores.clear();

	std::ifstream file{ m_FilePath };
	if (!file.is_open())
	{
		return;
	}

	std::string name{};
	int score{};

	while (file >> name >> score)
	{
		m_HighScores.push_back(HighScoreEntry{ name, score });
	}

	SortAndTrim();
}

void HighScoreManager::Save() const
{
	std::ofstream file{ m_FilePath };
	if (!file.is_open())
	{
		return;
	}

	for (const auto& entry : m_HighScores)
	{
		file << entry.name << ' ' << entry.score << '\n';
	}
}

void HighScoreManager::SortAndTrim()
{
	std::sort(
		m_HighScores.begin(),
		m_HighScores.end(),
		[](const HighScoreEntry& lhs, const HighScoreEntry& rhs)
		{
			return lhs.score > rhs.score;
		}
	);

	if (m_HighScores.size() > static_cast<size_t>(m_MaxEntries))
	{
		m_HighScores.resize(static_cast<size_t>(m_MaxEntries));
	}
}