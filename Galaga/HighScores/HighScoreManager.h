#pragma once

#include "HighScoreEntry.h"

#include <string>
#include <vector>

class HighScoreManager final
{
public:
	explicit HighScoreManager(std::string filePath);

	void AddScore(const std::string& name, int score);
	std::vector<HighScoreEntry> GetHighScores() const;

private:
	void Load();
	void Save() const;
	void SortAndTrim();

	std::string m_FilePath{};
	std::vector<HighScoreEntry> m_HighScores{};

	static constexpr int m_MaxEntries{ 5 };
};