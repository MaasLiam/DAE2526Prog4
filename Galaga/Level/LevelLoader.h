#pragma once

namespace dae
{
	class Scene;
}

class LevelLoader final
{
public:
	static void LoadStage(dae::Scene& scene, int stageIndex);

	LevelLoader() = delete;
};