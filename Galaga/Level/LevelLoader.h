#pragma once

namespace dae
{
	class Scene;
}

namespace galaga
{
	class GalagaGameControllerComponent;

	class LevelLoader final
	{
	public:
		static void LoadStage(dae::Scene& scene, int stageIndex, GalagaGameControllerComponent& gameController);
		static void ClearStage(dae::Scene& scene);

		~LevelLoader() = delete;
		LevelLoader(const LevelLoader&) = delete;
		LevelLoader(LevelLoader&&) = delete;
		LevelLoader& operator=(const LevelLoader&) = delete;
		LevelLoader& operator=(LevelLoader&&) = delete;

		LevelLoader() = delete;
	};
}