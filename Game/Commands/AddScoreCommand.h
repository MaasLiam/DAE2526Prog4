#pragma once
#include "Command.h"

namespace dae
{
	class GameObject;

	class AddScoreCommand final : public Command
	{
	public:
		explicit AddScoreCommand(GameObject& target);
		void Execute(float) override;

	private:
		GameObject& m_Target;
	};
}