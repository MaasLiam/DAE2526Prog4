#pragma once
#include <memory>
#include <vector>
#include <algorithm>
#include "Binding.h"

namespace dae
{
	class Command;

	class InputDevice
	{
	public:
		virtual ~InputDevice() = default;

		void AddBinding(std::unique_ptr<Command> command, InputKey key, InputState state)
		{
			m_Bindings.push_back(Binding{ std::move(command), key, state });
		}

		void RemoveBinding(InputKey key, InputState state)
		{
			std::erase_if(m_Bindings,
				[key, state](const Binding& binding)
				{
					return binding.key == key && binding.state == state;
				});
		}

		void ProcessInput(float deltaTime)
		{
			UpdateState();

			for (auto& binding : m_Bindings)
			{
				if (binding.command && IsTriggered(binding.key, binding.state))
				{
					binding.command->Execute(deltaTime);
				}
			}

			FinalizeFrame();
		}

	protected:
		virtual void UpdateState() = 0;
		virtual bool IsTriggered(InputKey key, InputState state) const = 0;
		virtual void FinalizeFrame() {}

	private:
		std::vector<Binding> m_Bindings{};
	};
}