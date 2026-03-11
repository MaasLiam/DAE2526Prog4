#pragma once

namespace dae
{
	enum class InputState
	{
		Down,
		Up,
		Pressed
	};

	enum class InputKey
	{
		// Keyboard
		W,
		A,
		S,
		D,

		// Controller
		DPadUp,
		DPadDown,
		DPadLeft,
		DPadRight,

		ButtonA,
		ButtonB,
		ButtonX,
		ButtonY,

		Start,
		Back,

		LeftShoulder,
		RightShoulder,

		LeftThumb,
		RightThumb,

		LeftTrigger,
		RightTrigger
	};
}