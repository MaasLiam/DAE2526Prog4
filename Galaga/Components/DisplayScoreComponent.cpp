#include "DisplayScoreComponent.h"
#include "ScoreComponent.h"
#include "TextComponent.h"
#include "GameObject.h"
#include "Event.h"


galaga::DisplayScoreComponent::DisplayScoreComponent(dae::GameObject* owner, galaga::ScoreComponent& targetScore, std::string label)
	: Component(owner)
	, m_TargetScore(&targetScore)
	, m_TextComponent(owner->GetComponent<dae::TextComponent>())
	, m_TargetSubject(&targetScore.GetSubject())
	, m_Label(std::move(label))
{
	m_TargetSubject->AddObserver(this);
	UpdateText();
}

void galaga::DisplayScoreComponent::Notify(dae::Event event, dae::GameObject*)
{
	if (event == dae::Event::ScoreChanged)
	{
		UpdateText();
	}
}

galaga::DisplayScoreComponent::~DisplayScoreComponent()
{
	if (m_TargetSubject == nullptr)
	{
		return;
	}

	m_TargetSubject->RemoveObserver(this);
}

void galaga::DisplayScoreComponent::OnSubjectDestroyed(dae::Subject* subject)
{
	if (subject != m_TargetSubject)
	{
		return;
	}

	m_TargetSubject = nullptr;
	m_TargetScore = nullptr;
}

void galaga::DisplayScoreComponent::UpdateText()
{
	if (m_TextComponent == nullptr || m_TargetScore == nullptr)
	{
		return;
	}
	m_TextComponent->SetText(m_Label + " Score: " + std::to_string(m_TargetScore->GetScore()));
}