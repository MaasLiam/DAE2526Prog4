#pragma once

#include "SoundSystem.h"

namespace galaga
{
	enum class SoundIds : dae::SoundId
	{
		Shoot = 0,
		StageStart = 1,
		TractorBeam = 2,
		GameOver = 3,
		BossGalagaDestroyed = 4,
		BossGalagaInjured = 5,
		EnemyDestroyed = 6,
		EnemyDive = 7,
		PlayerHit = 8
	};

	constexpr dae::SoundId ToSoundId(SoundIds soundId)
	{
		return static_cast<dae::SoundId>(soundId);
	}
}