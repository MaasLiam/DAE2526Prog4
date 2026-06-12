#pragma once
#include "Component.h"
#include "Subject.h"
#include "Observer.h"

namespace galaga
{
	class ScoreComponent final : public dae::Component, public dae::Observer
	{
	public:
		ScoreComponent(dae::GameObject* owner);
		void Notify(dae::Event event, dae::GameObject* gameObject) override;
		void AddScore(int score);
		void Reset();
		int GetScore() const;
		dae::Subject& GetSubject();

		~ScoreComponent() override = default;

		ScoreComponent(const ScoreComponent&) = delete;
		ScoreComponent(ScoreComponent&&) = delete;
		ScoreComponent& operator=(const ScoreComponent&) = delete;
		ScoreComponent& operator=(ScoreComponent&&) = delete;

	private:
		int m_Score{ 0 };
		dae::Subject m_Subject{};
	};
}