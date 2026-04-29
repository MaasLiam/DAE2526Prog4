#include "DisplayScoreComponent.h"
#include "ScoreComponent.h"
#include "TextComponent.h"
#include "GameObject.h"
#include "Event.h"



namespace dae
{
	DisplayScoreComponent::DisplayScoreComponent(GameObject* owner, ScoreComponent& targetScore, std::string label)
		: Component(owner)
		, m_targetScore(&targetScore)
		, m_Label(std::move(label))
	{
		m_TextComponent = GetOwner()->GetComponent<TextComponent>();
		m_targetScore->GetSubject().AddObserver(this);
		UpdateText();
	}

	void DisplayScoreComponent::Notify(Event event, GameObject*)
	{
		if (event == Event::ScoreChanged)
		{
			UpdateText();
		}
	}

	void DisplayScoreComponent::UpdateText()
	{
		if (m_TextComponent == nullptr || m_targetScore == nullptr)
		{
			return;
		}
		m_TextComponent->SetText(m_Label + " Score: " + std::to_string(m_targetScore->GetScore()));
	}

}