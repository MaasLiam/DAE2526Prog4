#include "LoggingSoundSystem.h"
#include <iostream>

namespace dae
{
	LoggingSoundSystem::LoggingSoundSystem(std::unique_ptr<SoundSystem>&& realSoundSystem)
		: m_RealSoundSystem(std::move(realSoundSystem))
	{
	}

	void LoggingSoundSystem::Play(SoundId id, float volume)
	{
		m_RealSoundSystem->Play(id, volume);
		std::cout << "[Sound] Play ID: " << id << " Volume: " << volume << std::endl;
	}

	void LoggingSoundSystem::Load(SoundId id, const std::string& filePath)
	{
		m_RealSoundSystem->Load(id, filePath);
		std::cout << "[Sound] Load ID: " << id << " Path: " << filePath << std::endl;
	}

	void LoggingSoundSystem::ToggleMute()
	{
		m_RealSoundSystem->ToggleMute();
		std::cout << "[Sound] Muted: " << std::boolalpha << m_RealSoundSystem->IsMuted() << std::endl;
	}

	bool LoggingSoundSystem::IsMuted() const
	{
		return m_RealSoundSystem->IsMuted();
	}

	void LoggingSoundSystem::StopAll()
	{
		m_RealSoundSystem->StopAll();
		std::cout << "[Sound] Stop all" << std::endl;
	}
}