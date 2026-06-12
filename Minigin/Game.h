#pragma once

class Game
{
public:
	Game() = default;
	virtual ~Game() = default;

	Game(const Game&) = delete;
	Game(Game&&) = delete;
	Game& operator=(const Game&) = delete;
	Game& operator=(Game&&) = delete;

	virtual void Initialize() = 0;
	virtual void Update(float) {}
	virtual void Render() {}
};