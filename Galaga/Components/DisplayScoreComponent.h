#pragma once

#include "Component.h"
#include "Observer.h"

#include <string>

namespace dae
{
	class TextComponent;
	enum class Event;
}

namespace galaga
{
	class ScoreComponent;

	class DisplayScoreComponent final : public dae::Component, public dae::Observer
	{
	public:
		DisplayScoreComponent(dae::GameObject* owner, ScoreComponent& targetScore, std::string label);
		void Notify(dae::Event event, dae::GameObject* gameObject) override;
		~DisplayScoreComponent() override;
		void OnSubjectDestroyed(dae::Subject* subject) override;

		DisplayScoreComponent(const DisplayScoreComponent&) = delete;
		DisplayScoreComponent(DisplayScoreComponent&&) = delete;
		DisplayScoreComponent& operator=(const DisplayScoreComponent&) = delete;
		DisplayScoreComponent& operator=(DisplayScoreComponent&&) = delete;

	private:
		void UpdateText();

		ScoreComponent* m_TargetScore{ nullptr };
		dae::TextComponent* m_TextComponent{ nullptr };
		std::string m_Label;
		dae::Subject* m_TargetSubject{};
	};
}