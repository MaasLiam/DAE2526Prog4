#pragma once
#include <memory>
#include "Binding.h"

namespace dae
{
	class Command;

	class ControllerInput final
	{
	public:
		explicit ControllerInput(unsigned int controllerIndex);
		~ControllerInput();

		ControllerInput(const ControllerInput&) = delete;
		ControllerInput(ControllerInput&&) = delete;
		ControllerInput& operator=(const ControllerInput&) = delete;
		ControllerInput& operator=(ControllerInput&&) = delete;

		void AddBinding(std::unique_ptr<Command> command, InputKey key, InputState state);
		void RemoveBinding(InputKey key, InputState state);
		void ProcessInput(float deltaTime);

	private:
		class Impl;
		std::unique_ptr<Impl> m_pImpl;
	};
}