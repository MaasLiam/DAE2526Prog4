#pragma once
#include "Command.h"

namespace dae
{
	class GameObject;
	class AddScoreCommand final : public Command
	{
	public:
		AddScoreCommand(GameObject& target, int scoreToAdd);
		void Execute(float) override;
	private:
		GameObject& m_Target;
		int m_ScoreToAdd;
	};
}