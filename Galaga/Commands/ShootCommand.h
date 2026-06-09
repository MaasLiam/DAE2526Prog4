#pragma once

#include "Command.h"
#include "GameObject.h"

namespace dae
{
	class Scene;
}

class GalagaGameControllerComponent;

class ShootCommand final : public dae::Command
{
public:
	ShootCommand(
		dae::GameObject& shooter,
		dae::Scene& scene,
		GalagaGameControllerComponent* gameController = nullptr
	);

	void Execute(float deltaTime) override;

private:
	dae::GameObject& m_Shooter;
	dae::Scene& m_Scene;
	GalagaGameControllerComponent* m_GameController{};
};