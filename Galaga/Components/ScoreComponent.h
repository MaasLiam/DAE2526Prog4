#pragma once

#include "Component.h"
#include "Observer.h"
#include "Subject.h"

namespace galaga
{
	class ScoreComponent final : public dae::Component, public dae::Observer
	{
	public:
		explicit ScoreComponent(dae::GameObject* owner);

		~ScoreComponent() override = default;

		ScoreComponent(const ScoreComponent&) = delete;
		ScoreComponent(ScoreComponent&&) = delete;
		ScoreComponent& operator=(const ScoreComponent&) = delete;
		ScoreComponent& operator=(ScoreComponent&&) = delete;

		void Notify(dae::Event event, dae::GameObject* sender) override;

		void AddScore(int score);
		void Reset();

		[[nodiscard]] int GetScore() const;
		[[nodiscard]] dae::Subject& GetSubject();

	private:
		int m_Score{};
		dae::Subject m_Subject{};
	};
}