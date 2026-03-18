#pragma once
#include "Command.h"

namespace dae
{
	class GameObject;

	class DamagePlayerCommand final : public Command
	{
	public:
		DamagePlayerCommand(GameObject& player);

		void Execute(float) override;

	private:
		GameObject& m_Player;
	};
}