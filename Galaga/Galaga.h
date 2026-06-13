#pragma once

#include "Game.h"

class Galaga final : public Game
{
public:
	Galaga() = default;
	~Galaga() override = default;

	Galaga(const Galaga&) = delete;
	Galaga(Galaga&&) = delete;
	Galaga& operator=(const Galaga&) = delete;
	Galaga& operator=(Galaga&&) = delete;

	void Initialize() override;
};