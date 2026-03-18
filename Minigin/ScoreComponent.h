#pragma once
#include "Component.h"
#include "Subject.h"

namespace dae
{
	class ScoreComponent final : public Component
	{
	public:
		ScoreComponent(GameObject* owner);

		void AddScore(int score);
		int GetScore() const;

		Subject& GetSubject();

	private:
		int m_Score{ 0 };
		Subject m_Subject{};
	};
}