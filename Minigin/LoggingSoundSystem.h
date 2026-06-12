#pragma once

#include <memory>
#include "SoundSystem.h"

namespace dae
{
	class LoggingSoundSystem final : public SoundSystem
	{
	public:
		explicit LoggingSoundSystem(std::unique_ptr<SoundSystem>&& realSoundSystem);

		~LoggingSoundSystem() override = default;

		LoggingSoundSystem(const LoggingSoundSystem&) = delete;
		LoggingSoundSystem(LoggingSoundSystem&&) = delete;
		LoggingSoundSystem& operator=(const LoggingSoundSystem&) = delete;
		LoggingSoundSystem& operator=(LoggingSoundSystem&&) = delete;

		void Play(SoundId id, float volume) override;
		void Load(SoundId id, const std::string& filePath) override;

		void ToggleMute() override;
		bool IsMuted() const override;
		void StopAll() override;

	private:
		std::unique_ptr<SoundSystem> m_RealSoundSystem;
	};
}