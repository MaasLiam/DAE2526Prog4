#pragma once
#include "Singleton.h"
#include <array>
#include <memory>

namespace dae
{
	class KeyboardInput;
	class ControllerInput;

	class InputManager final : public Singleton<InputManager>
	{
	public:
		InputManager();
		~InputManager();

		bool ProcessInput(float deltaTime);

		KeyboardInput* GetKeyboardInput() const;
		ControllerInput* GetControllerInput(unsigned int index) const;

	private:
		std::unique_ptr<KeyboardInput> m_pKeyboardInput{};
		std::array<std::unique_ptr<ControllerInput>, 4> m_pControllers{};
	};
}