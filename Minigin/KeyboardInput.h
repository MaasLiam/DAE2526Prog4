#pragma once
#include <vector>
#include "InputDevice.h"

namespace dae
{
	class KeyboardInput final : public InputDevice
	{
	public:
		KeyboardInput();

	protected:
		void UpdateState() override;
		bool IsTriggered(InputKey key, InputState state) const override;
		void FinalizeFrame() override;

	private:
		int ToSDLScancode(InputKey key) const;

		const bool* m_pCurrentState{};
		int m_NumKeys{};
		std::vector<bool> m_PreviousKeyboardState{};
	};
}