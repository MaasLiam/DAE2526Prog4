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
	class HealthComponent;

	class DisplayLivesComponent final : public dae::Component, public dae::Observer
	{
	public:
		DisplayLivesComponent(dae::GameObject* owner, HealthComponent& targetHealth, std::string label);
		void Notify(dae::Event event, dae::GameObject* sender) override;

		~DisplayLivesComponent() override;
		void OnSubjectDestroyed(dae::Subject* subject) override;

		DisplayLivesComponent(const DisplayLivesComponent&) = delete;
		DisplayLivesComponent(DisplayLivesComponent&&) = delete;
		DisplayLivesComponent& operator=(const DisplayLivesComponent&) = delete;
		DisplayLivesComponent& operator=(DisplayLivesComponent&&) = delete;

	private:
		void UpdateText();

		HealthComponent* m_TargetHealth{};
		dae::TextComponent* m_TextComponent{};
		dae::Subject* m_TargetSubject{};
		std::string m_Label{};
	};
}