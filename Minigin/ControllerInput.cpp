#include "ControllerInput.h"
#include <algorithm>
#include <vector>
#include "Command.h"
#include "InputDevice.h"

#ifdef _WIN32
#include <Windows.h>
#include <Xinput.h>
#pragma comment(lib, "xinput.lib")
#else
#include <SDL3/SDL.h>
#endif

namespace
{
#ifdef _WIN32
	WORD ToXInputButton(dae::InputKey key)
	{
		switch (key)
		{
		case dae::InputKey::DPadUp:
			return XINPUT_GAMEPAD_DPAD_UP;
		case dae::InputKey::DPadDown:
			return XINPUT_GAMEPAD_DPAD_DOWN;
		case dae::InputKey::DPadLeft:
			return XINPUT_GAMEPAD_DPAD_LEFT;
		case dae::InputKey::DPadRight:
			return XINPUT_GAMEPAD_DPAD_RIGHT;
		case dae::InputKey::ButtonA:
			return XINPUT_GAMEPAD_A;
		case dae::InputKey::ButtonB:
			return XINPUT_GAMEPAD_B;
		case dae::InputKey::ButtonX:
			return XINPUT_GAMEPAD_X;
		case dae::InputKey::ButtonY:
			return XINPUT_GAMEPAD_Y;
		case dae::InputKey::Start:
			return XINPUT_GAMEPAD_START;
		case dae::InputKey::Back:
			return XINPUT_GAMEPAD_BACK;
		case dae::InputKey::LeftShoulder:
			return XINPUT_GAMEPAD_LEFT_SHOULDER;
		case dae::InputKey::RightShoulder:
			return XINPUT_GAMEPAD_RIGHT_SHOULDER;
		case dae::InputKey::LeftThumb:
			return XINPUT_GAMEPAD_LEFT_THUMB;
		case dae::InputKey::RightThumb:
			return XINPUT_GAMEPAD_RIGHT_THUMB;
		default:
			return 0;
		}
	}
#else
	unsigned int ToSDLStyleMask(dae::InputKey key)
	{
		switch (key)
		{
		case dae::InputKey::DPadUp:
			return 1u << 0;
		case dae::InputKey::DPadDown:
			return 1u << 1;
		case dae::InputKey::DPadLeft:
			return 1u << 2;
		case dae::InputKey::DPadRight:
			return 1u << 3;
		case dae::InputKey::ButtonA:
			return 1u << 4;
		case dae::InputKey::ButtonB:
			return 1u << 5;
		case dae::InputKey::ButtonX:
			return 1u << 6;
		case dae::InputKey::ButtonY:
			return 1u << 7;
		case dae::InputKey::Start:
			return 1u << 8;
		case dae::InputKey::Back:
			return 1u << 9;
		case dae::InputKey::LeftShoulder:
			return 1u << 10;
		case dae::InputKey::RightShoulder:
			return 1u << 11;
		case dae::InputKey::LeftThumb:
			return 1u << 12;
		case dae::InputKey::RightThumb:
			return 1u << 13;
		default:
			return 0;
		}
	}
#endif
}

class dae::ControllerInput::Impl final : public InputDevice
{
public:
	explicit Impl(unsigned int controllerIndex)
		: m_ControllerIndex(controllerIndex)
	{
#ifndef _WIN32
		OpenController();
#endif
	}

	~Impl()
	{
#ifndef _WIN32
		if (m_pGamepad)
		{
			SDL_CloseGamepad(m_pGamepad);
			m_pGamepad = nullptr;
		}
#endif
	}

protected:
	void UpdateState() override
	{
		m_PreviousButtons = m_CurrentButtons;
		m_CurrentButtons = 0;

#ifdef _WIN32
		XINPUT_STATE state{};
		const DWORD result = XInputGetState(m_ControllerIndex, &state);
		if (result != ERROR_SUCCESS)
			return;

		m_CurrentButtons = state.Gamepad.wButtons;
#else
		if (!m_pGamepad)
		{
			OpenController();
		}
		if (!m_pGamepad)
		{
			return;
		}
		if (!SDL_GamepadConnected(m_pGamepad))
		{
			return;
		}
		if (SDL_GetGamepadButton(m_pGamepad, SDL_GAMEPAD_BUTTON_DPAD_UP))
		{
			m_CurrentButtons |= ToSDLStyleMask(InputKey::DPadUp);
		}
		if (SDL_GetGamepadButton(m_pGamepad, SDL_GAMEPAD_BUTTON_DPAD_DOWN))
		{
			m_CurrentButtons |= ToSDLStyleMask(InputKey::DPadDown);
		}
		if (SDL_GetGamepadButton(m_pGamepad, SDL_GAMEPAD_BUTTON_DPAD_LEFT))
		{
			m_CurrentButtons |= ToSDLStyleMask(InputKey::DPadLeft);
		}
		if (SDL_GetGamepadButton(m_pGamepad, SDL_GAMEPAD_BUTTON_DPAD_RIGHT))
		{
			m_CurrentButtons |= ToSDLStyleMask(InputKey::DPadRight);
		}
		if (SDL_GetGamepadButton(m_pGamepad, SDL_GAMEPAD_BUTTON_SOUTH))
		{
			m_CurrentButtons |= ToSDLStyleMask(InputKey::ButtonA);
		}
		if (SDL_GetGamepadButton(m_pGamepad, SDL_GAMEPAD_BUTTON_EAST))
		{
			m_CurrentButtons |= ToSDLStyleMask(InputKey::ButtonB);
		}
		if (SDL_GetGamepadButton(m_pGamepad, SDL_GAMEPAD_BUTTON_WEST))
		{
			m_CurrentButtons |= ToSDLStyleMask(InputKey::ButtonX);
		}
		if (SDL_GetGamepadButton(m_pGamepad, SDL_GAMEPAD_BUTTON_NORTH))
		{
			m_CurrentButtons |= ToSDLStyleMask(InputKey::ButtonY);
		}
		if (SDL_GetGamepadButton(m_pGamepad, SDL_GAMEPAD_BUTTON_START))
		{
			m_CurrentButtons |= ToSDLStyleMask(InputKey::Start);
		}
		if (SDL_GetGamepadButton(m_pGamepad, SDL_GAMEPAD_BUTTON_BACK))
		{
			m_CurrentButtons |= ToSDLStyleMask(InputKey::Back);
		}
#endif
	}

	bool IsTriggered(InputKey key, InputState state) const override
	{
#ifdef _WIN32
		const unsigned int button = static_cast<unsigned int>(ToXInputButton(key));
#else
		const unsigned int button = ToSDLStyleMask(key);
#endif
		if (button == 0)
			return false;

		const bool isPressedNow = (m_CurrentButtons & button) != 0;
		const bool wasPressedBefore = (m_PreviousButtons & button) != 0;

		switch (state)
		{
		case InputState::Down:
			return isPressedNow && !wasPressedBefore;
		case InputState::Up:
			return !isPressedNow && wasPressedBefore;
		case InputState::Pressed:
			return isPressedNow;
		default:
			return false;
		}
	}

private:
#ifndef _WIN32
	void OpenController()
	{
		int gamepadCount{};
		SDL_JoystickID* joystickIds = SDL_GetJoysticks(&gamepadCount);
		if (!joystickIds)
			return;

		int foundGamepadIndex{};
		for (int i{}; i < gamepadCount; ++i)
		{
			if (SDL_IsGamepad(joystickIds[i]))
			{
				if (foundGamepadIndex == static_cast<int>(m_ControllerIndex))
				{
					m_pGamepad = SDL_OpenGamepad(joystickIds[i]);
					break;
				}
				++foundGamepadIndex;
			}
		}

		SDL_free(joystickIds);
	}
#endif

	unsigned int m_ControllerIndex{};
	unsigned int m_PreviousButtons{};
	unsigned int m_CurrentButtons{};

#ifndef _WIN32
	SDL_Gamepad* m_pGamepad{};
#endif
};

dae::ControllerInput::ControllerInput(unsigned int controllerIndex)
	: m_pImpl(std::make_unique<Impl>(controllerIndex))
{
}

dae::ControllerInput::~ControllerInput() = default;
dae::ControllerInput::ControllerInput(ControllerInput&&) noexcept = default;
dae::ControllerInput& dae::ControllerInput::operator=(ControllerInput&&) noexcept = default;

void dae::ControllerInput::AddBinding(std::unique_ptr<Command> command, InputKey key, InputState state)
{
	m_pImpl->AddBinding(std::move(command), key, state);
}

void dae::ControllerInput::RemoveBinding(InputKey key, InputState state)
{
	m_pImpl->RemoveBinding(key, state);
}

void dae::ControllerInput::ProcessInput(float deltaTime)
{
	m_pImpl->ProcessInput(deltaTime);
}