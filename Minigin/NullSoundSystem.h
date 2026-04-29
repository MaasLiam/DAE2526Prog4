#pragma once

#include "SoundSystem.h"

namespace dae
{
	class NullSoundSystem final : public SoundSystem
	{
	public:
		void Play(SoundId id, float volume) override;
		void Load(SoundId id, const std::string& filePath) override;
	};
}