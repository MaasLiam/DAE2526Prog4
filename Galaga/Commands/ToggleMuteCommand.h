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

		~ToggleMuteCommand() override = default;

		ToggleMuteCommand(const ToggleMuteCommand&) = delete;
		ToggleMuteCommand(ToggleMuteCommand&&) = delete;
		ToggleMuteCommand& operator=(const ToggleMuteCommand&) = delete;
		ToggleMuteCommand& operator=(ToggleMuteCommand&&) = delete;

	private:
		dae::SoundSystem& m_SoundSystem;
	};
}