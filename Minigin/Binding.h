#pragma once
#include "InputTypes.h"
#include <memory>
#include "Command.h"

namespace dae
{
	struct Binding
	{
		std::unique_ptr<Command> command;
		InputKey key;
		InputState state;
	};
}