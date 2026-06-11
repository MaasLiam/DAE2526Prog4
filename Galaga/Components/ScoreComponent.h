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
		void AddScore(int score);
		void Reset();
		int GetScore() const;
		Subject& GetSubject();

	private:
		int m_Score{ 0 };
		Subject m_Subject{};
	};
}