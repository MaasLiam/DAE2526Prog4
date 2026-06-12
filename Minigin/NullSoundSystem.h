#pragma once

#include "SoundSystem.h"

namespace dae
{
	class NullSoundSystem final : public SoundSystem
	{
	public:
		void Play(SoundId id, float volume) override;
		void Load(SoundId id, const std::string& filePath) override;
		void StopAll() override;

		NullSoundSystem() = default;
		~NullSoundSystem() override = default;

		NullSoundSystem(const NullSoundSystem&) = delete;
		NullSoundSystem(NullSoundSystem&&) = delete;
		NullSoundSystem& operator=(const NullSoundSystem&) = delete;
		NullSoundSystem& operator=(NullSoundSystem&&) = delete;

		void ToggleMute() override;
		bool IsMuted() const override;
	};
}