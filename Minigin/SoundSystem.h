#pragma once

#include <string>

namespace dae
{
	using SoundId = unsigned short;

	class SoundSystem
	{
	public:
		virtual ~SoundSystem() = default;

		virtual void Play(SoundId id, float volume) = 0;
		virtual void Load(SoundId id, const std::string& filePath) = 0;
	};
}