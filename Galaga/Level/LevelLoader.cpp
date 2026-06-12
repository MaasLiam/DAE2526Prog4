#include "LevelLoader.h"

#include "CollisionComponent.h"
#include "EnemyComponent.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "Scene.h"
#include "TransformComponent.h"
#include "BulletComponent.h"
#include "EnemyShooterComponent.h"
#include "GalagaGameControllerComponent.h"
#include "BossTractorBeamComponent.h"
#include "TractorBeamVisualComponent.h"
#include "CapturedFighterComponent.h"

#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace
{
	struct EnemySpawn
	{
		galaga::EnemyType type{};
		glm::vec3 formationPosition{};
	};

	galaga::EnemyType ParseEnemyType(const std::string& text)
	{
		if (text == "Bee")
		{
			return galaga::EnemyType::Bee;
		}

		if (text == "Butterfly")
		{
			return galaga::EnemyType::Butterfly;
		}

		if (text == "BossGalaga")
		{
			return galaga::EnemyType::BossGalaga;
		}

		throw std::runtime_error("Unknown enemy type in level file: " + text);
	}

	const char* GetEnemyTexture(galaga::EnemyType type)
	{
		switch (type)
		{
		case galaga::EnemyType::Bee:
			return "Sprites/Bee01.png";

		case galaga::EnemyType::Butterfly:
			return "Sprites/Butterfly01.png";

		case galaga::EnemyType::BossGalaga:
			return "Sprites/BossGalaga.png";
		}

		return "Sprites/Bee01.png";
	}

	std::string GetStagePath(int stageIndex)
	{
		return "Data/Levels/stage_" + std::to_string(stageIndex) + ".txt";
	}

	void RemoveCurrentStageObjects(dae::Scene& scene)
	{
		for (const auto& object : scene.GetObjects())
		{
			if (object->GetComponent<EnemyComponent>() || object->GetComponent<BulletComponent>() || object->GetComponent<TractorBeamVisualComponent>() || object->GetComponent<CapturedFighterComponent>())
			{
				scene.Remove(*object);
			}
		}
	}

	std::vector<EnemySpawn> ReadEnemySpawns(int stageIndex)
	{
		const auto path = GetStagePath(stageIndex);

		std::ifstream file{ path };
		if (!file.is_open())
		{
			throw std::runtime_error("Could not open level file: " + path);
		}

		std::vector<EnemySpawn> enemySpawns{};

		std::string enemyTypeText{};
		int amount{};
		float startX{};
		float y{};
		float spacing{};

		while (file >> enemyTypeText >> amount >> startX >> y >> spacing)
		{
			const auto type = ParseEnemyType(enemyTypeText);

			for (int index = 0; index < amount; ++index)
			{
				enemySpawns.push_back(
					EnemySpawn
					{
						type,
						glm::vec3
						{
							startX + static_cast<float>(index) * spacing, y, 0.f
						}
					}
				);
			}
		}

		return enemySpawns;
	}

	void SpawnEnemies(dae::Scene& scene, const std::vector<EnemySpawn>& enemySpawns, GalagaGameControllerComponent& gameController)
	{
		for (size_t index = 0; index < enemySpawns.size(); ++index)
		{
			const auto& spawn = enemySpawns[index];

			auto enemy = std::make_unique<dae::GameObject>();
			enemy->AddComponent<dae::TransformComponent>();

			const bool entersFromLeft = index % 2 == 0;
			const float spawnX = entersFromLeft ? -80.f : 720.f;
			const float spawnY = -60.f - static_cast<float>(index % 8) * 28.f;

			enemy->GetComponent<dae::TransformComponent>()->SetLocalPosition(spawnX, spawnY, 0.f);

			enemy->AddComponent<dae::RenderComponent>(GetEnemyTexture(spawn.type));
			enemy->AddComponent<CollisionComponent>(32.f, 32.f);
			enemy->AddComponent<EnemyComponent>(spawn.type);
			enemy->AddComponent<EnemyShooterComponent>(scene, gameController);

			if (spawn.type == galaga::EnemyType::BossGalaga)
			{
				enemy->AddComponent<BossTractorBeamComponent>(scene, gameController);
			}

			auto* enemyComponent = enemy->GetComponent<EnemyComponent>();
			enemyComponent->FlyIntoFormation(spawn.formationPosition);

			scene.Add(std::move(enemy));
		}
	}
}

void LevelLoader::LoadStage(dae::Scene& scene, int stageIndex, GalagaGameControllerComponent& gameController)
{
	ClearStage(scene);

	const auto enemySpawns = ReadEnemySpawns(stageIndex);
	SpawnEnemies(scene, enemySpawns, gameController);
}

void LevelLoader::ClearStage(dae::Scene& scene)
{
	RemoveCurrentStageObjects(scene);
}