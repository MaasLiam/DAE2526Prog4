#include "InputManager.h"
#include "KeyboardInput.h"
#include "ControllerInput.h"
#include <SDL3/SDL.h>
#include <backends/imgui_impl_sdl3.h>

dae::InputManager::InputManager()
	: m_pKeyboardInput(std::make_unique<KeyboardInput>())
{
	for (unsigned int i{}; i < m_pControllers.size(); ++i)
	{
		m_pControllers[i] = std::make_unique<ControllerInput>(i);
	}
}

dae::InputManager::~InputManager() = default;

bool dae::InputManager::ProcessInput(float deltaTime)
{
	SDL_Event e{};
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_EVENT_QUIT)
			return false;

		ImGui_ImplSDL3_ProcessEvent(&e);
	}

	if (m_pKeyboardInput)
	{
		m_pKeyboardInput->ProcessInput(deltaTime);
	}

	for (auto& controller : m_pControllers)
	{
		if (controller)
		{
			controller->ProcessInput(deltaTime);
		}
	}

	return true;
}

dae::KeyboardInput* dae::InputManager::GetKeyboardInput() const
{
	return m_pKeyboardInput.get();
}

dae::ControllerInput* dae::InputManager::GetControllerInput(unsigned int index) const
{
	if (index >= m_pControllers.size())
		return nullptr;

	return m_pControllers[index].get();
}