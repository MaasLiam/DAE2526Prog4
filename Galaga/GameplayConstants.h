#pragma once

#include <glm/vec3.hpp>

namespace galaga::gameplay
{
	inline constexpr float PlayerOneStartX{ 360.f };
	inline constexpr float PlayerTwoStartX{ 440.f };
	inline constexpr float PlayerStartY{ 500.f };
	inline constexpr float VersusBossStartX{ 400.f };
	inline constexpr float VersusBossStartY{ 90.f };
	inline constexpr float TractorBeamY{ 350.f };

	inline const glm::vec3 PlayerOneStartPosition{ PlayerOneStartX, PlayerStartY, 0.f };
	inline const glm::vec3 PlayerTwoStartPosition{ PlayerTwoStartX, PlayerStartY, 0.f };
	inline const glm::vec3 VersusBossStartPosition{ VersusBossStartX, VersusBossStartY, 0.f };

	inline constexpr float TractorBeamVisualOffsetX{ -15.f };
	inline constexpr float TractorBeamVisualOffsetY{ 32.f };
	inline constexpr float TractorBeamHitboxLeftOffset{ -15.f };
	inline constexpr float TractorBeamHitboxTopOffset{ 32.f };
	inline constexpr float TractorBeamHitboxWidth{ 64.f };
	inline constexpr float TractorBeamHitboxHeight{ 300.f };

	inline const glm::vec3 TractorBeamVisualOffset{ TractorBeamVisualOffsetX, TractorBeamVisualOffsetY, 0.f };

	inline constexpr float FighterCollisionWidth{ 32.f };
	inline constexpr float FighterCollisionHeight{ 32.f };
	inline constexpr float BossCollisionWidth{ 40.f };
	inline constexpr float BossCollisionHeight{ 32.f };

	inline constexpr float CapturedFighterPlayerOneOffsetX{ -24.f };
	inline constexpr float CapturedFighterPlayerTwoOffsetX{ 24.f };
	inline constexpr float CapturedFighterOffsetY{ 36.f };

	inline constexpr const char* FighterSprite{ "Sprites/fighter01.png" };
	inline constexpr const char* CapturedFighterSprite{ "Sprites/fighter02.png" };
	inline constexpr const char* TractorBeamSprite{ "Sprites/TractorBeam.png" };

	inline constexpr float PlayerHitboxOffsetX{ -20.f };
	inline constexpr float PlayerHitboxOffsetY{ 0.f };

	inline constexpr float EnemyHitboxOffsetX{ -8.f };
	inline constexpr float EnemyHitboxOffsetY{ 0.f };

	inline constexpr float BossHitboxOffsetX{ -10.f };
	inline constexpr float BossHitboxOffsetY{ 0.f };

	inline constexpr float BulletHitboxOffsetX{ -4.f };
	inline constexpr float BulletHitboxOffsetY{ 0.f };

	inline constexpr float PlayerBulletSpawnOffsetX{ 0.f };
	inline constexpr float PlayerBulletSpawnOffsetY{ -10.f };

	inline constexpr float EnemyBulletSpawnOffsetX{ -2.f };
	inline constexpr float EnemyBulletSpawnOffsetY{ 28.f };
}