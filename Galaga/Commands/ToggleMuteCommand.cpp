#include "ToggleMuteCommand.h"

#include "SoundSystem.h"

galaga::ToggleMuteCommand::ToggleMuteCommand(dae::SoundSystem& soundSystem)
	: m_SoundSystem(soundSystem)
{

}

void galaga::ToggleMuteCommand::Execute(float)
{
	m_SoundSystem.ToggleMute();
}