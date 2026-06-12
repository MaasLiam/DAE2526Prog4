#pragma once

#include "Command.h"

namespace dae
{
	class SoundSystem;
}

namespace galaga
{
	class ToggleMuteCommand final : public dae::Command
	{
	public:
		explicit ToggleMuteCommand(dae::SoundSystem& soundSystem);
		void Execute(float deltaTime) override;

	private:
		dae::SoundSystem& m_SoundSystem;
	};
}