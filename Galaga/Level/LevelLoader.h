#pragma once

namespace dae
{
	class Scene;
}

class LevelLoader final
{
public:
	static void LoadStage(dae::Scene& scene, int stageIndex);
	static void ClearStage(dae::Scene& scene);

	LevelLoader() = delete;
};