#pragma once
#include <string>
#include "Component.h"
#include "Observer.h"

namespace dae
{
	class HealthComponent;
	class TextComponent;
	enum class Event;
	class DisplayLivesComponent final : public Component, public Observer
	{
	public:
		DisplayLivesComponent(GameObject* owner, HealthComponent& targetHealth, std::string label);
		void Notify(Event event, GameObject* sender) override;

	private:
		void UpdateText();

		HealthComponent* m_TargetHealth{ nullptr };
		TextComponent* m_TextComponent{ nullptr };
		std::string m_Label;
	};
}