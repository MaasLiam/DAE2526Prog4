#include "ToggleMuteCommand.h"

#include "SoundSystem.h"

ToggleMuteCommand::ToggleMuteCommand(dae::SoundSystem& soundSystem)
	: m_SoundSystem(soundSystem)
{}

void ToggleMuteCommand::Execute(float)
{
	m_SoundSystem.ToggleMute();
}