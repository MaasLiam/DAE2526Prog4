#pragma once

#include "SoundSystem.h"

#include <memory>

namespace dae
{
	class SDLSoundSystem final : public SoundSystem
	{
	public:
		SDLSoundSystem();
		~SDLSoundSystem() override;

		SDLSoundSystem(const SDLSoundSystem& other) = delete;
		SDLSoundSystem(SDLSoundSystem&& other) = delete;
		SDLSoundSystem& operator=(const SDLSoundSystem& other) = delete;
		SDLSoundSystem& operator=(SDLSoundSystem&& other) = delete;

		void Play(SoundId id, float volume) override;
		void Load(SoundId id, const std::string& filePath) override;

	private:
		class Impl;
		std::unique_ptr<Impl> m_Impl;
	};
}