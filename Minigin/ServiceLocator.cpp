#include "ServiceLocator.h"
#include "NullSoundSystem.h"

namespace dae
{
	std::unique_ptr<SoundSystem> ServiceLocator::m_SoundSystem =
		std::make_unique<NullSoundSystem>();

	SoundSystem& ServiceLocator::GetSoundSystem()
	{
		return *m_SoundSystem;
	}

	void ServiceLocator::RegisterSoundSystem(std::unique_ptr<SoundSystem>&& soundSystem)
	{
		if (soundSystem == nullptr)
		{
			m_SoundSystem = std::make_unique<NullSoundSystem>();
		}
		else
		{
			m_SoundSystem = std::move(soundSystem);
		}
	}
}