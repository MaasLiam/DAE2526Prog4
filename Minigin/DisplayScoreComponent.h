#pragma once
#include "Component.h"
#include "Observer.h"
#include <string>

namespace dae
{
	class ScoreComponent;
	class TextComponent;
	enum class Event;
	
	class DisplayScoreComponent final : public Component, public Observer
	{
	public:
		DisplayScoreComponent(GameObject* owner, ScoreComponent& targetScore, std::string label);
		void Notify(Event event, GameObject*) override;

	private:
		void UpdateText();

		ScoreComponent* m_targetScore{ nullptr };
		TextComponent* m_TextComponent{ nullptr };
		std::string m_Label;
	};
}