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

	private:
		void UpdateText();

		HealthComponent* m_TargetHealth{ nullptr };
		dae::TextComponent* m_TextComponent{ nullptr };
		std::string m_Label;
	};
}