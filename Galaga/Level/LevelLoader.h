#pragma once

namespace dae
{
	class Scene;
}

class GalagaGameControllerComponent;
class LevelLoader final
{
public:
	static void LoadStage(dae::Scene& scene, int stageIndex, GalagaGameControllerComponent& gameController);
	static void ClearStage(dae::Scene& scene);

	LevelLoader() = delete;
};