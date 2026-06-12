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

	~HighScoreManager() = default;
	HighScoreManager(const HighScoreManager&) = delete;
	HighScoreManager(HighScoreManager&&) = delete;
	HighScoreManager& operator=(const HighScoreManager&) = delete;
	HighScoreManager& operator=(HighScoreManager&&) = delete;

private:
	void Load();
	void Save() const;
	void SortAndTrim();

	std::string m_FilePath{};
	std::vector<HighScoreEntry> m_HighScores{};

	static constexpr int MaxEntries{ 5 };
};