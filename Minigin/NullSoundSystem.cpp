#include "NullSoundSystem.h"

namespace dae
{
	void NullSoundSystem::Play(SoundId, float)
	{}

	void NullSoundSystem::Load(SoundId, const std::string&)
	{}

	void NullSoundSystem::StopAll()
	{}

	void NullSoundSystem::ToggleMute()
	{}

	bool NullSoundSystem::IsMuted() const
	{
		return true;
	}
}