#pragma once
#include "Component.h"
#include "Subject.h"
#include "Observer.h"

namespace dae
{
	class ScoreComponent final : public Component, public Observer
	{
	public:
		ScoreComponent(GameObject* owner);

		void Notify(Event event, GameObject* gameObject) override;

		int GetScore() const;
		Subject& GetSubject();

	private:
		void AddScore(int score);

		int m_Score{ 0 };
		Subject m_Subject{};
	};
}