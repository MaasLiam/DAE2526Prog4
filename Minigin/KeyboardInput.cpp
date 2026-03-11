#include "KeyboardInput.h"
#include <SDL3/SDL.h>

dae::KeyboardInput::KeyboardInput()
{
	const int keyCount = SDL_SCANCODE_COUNT;
	m_PreviousKeyboardState.resize(static_cast<size_t>(keyCount), false);
}

void dae::KeyboardInput::UpdateState()
{
	m_pCurrentState = SDL_GetKeyboardState(&m_NumKeys);

	if (static_cast<int>(m_PreviousKeyboardState.size()) != m_NumKeys)
	{
		m_PreviousKeyboardState.resize(static_cast<size_t>(m_NumKeys), false);
	}
}

bool dae::KeyboardInput::IsTriggered(InputKey key, InputState state) const
{
	const int scancode = ToSDLScancode(key);
	if (scancode < 0 || scancode >= m_NumKeys)
		return false;

	const bool isPressedNow = m_pCurrentState[scancode];
	const bool wasPressedBefore = m_PreviousKeyboardState[static_cast<size_t>(scancode)];

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

void dae::KeyboardInput::FinalizeFrame()
{
	for (int i{}; i < m_NumKeys; ++i)
	{
		m_PreviousKeyboardState[static_cast<size_t>(i)] = m_pCurrentState[i];
	}
}

int dae::KeyboardInput::ToSDLScancode(InputKey key) const
{
	switch (key)
	{
	case InputKey::W: return SDL_SCANCODE_W;
	case InputKey::A: return SDL_SCANCODE_A;
	case InputKey::S: return SDL_SCANCODE_S;
	case InputKey::D: return SDL_SCANCODE_D;

	default:
		return -1;
	}
}