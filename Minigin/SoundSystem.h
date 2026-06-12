#pragma once

#include <string>

namespace dae
{
	using SoundId = unsigned short;

	class SoundSystem
	{
	public:
		SoundSystem() = default;
		virtual ~SoundSystem() = default;

		SoundSystem(const SoundSystem&) = delete;
		SoundSystem(SoundSystem&&) = delete;
		SoundSystem& operator=(const SoundSystem&) = delete;
		SoundSystem& operator=(SoundSystem&&) = delete;

		virtual void Play(SoundId id, float volume) = 0;
		virtual void Load(SoundId id, const std::string& filePath) = 0;
		virtual void StopAll() = 0;

		virtual void ToggleMute() = 0;
		virtual bool IsMuted() const = 0;
	};
}