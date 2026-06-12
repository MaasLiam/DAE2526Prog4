#include "DisplayLivesComponent.h"
#include "HealthComponent.h"
#include "TextComponent.h"
#include "Event.h"
#include "GameObject.h"

galaga::DisplayLivesComponent::DisplayLivesComponent(dae::GameObject* owner, HealthComponent& targetHealth, std::string label)
	: Component(owner)
	, m_TargetHealth(&targetHealth)
	, m_Label(std::move(label))
{

	m_TextComponent = GetOwner()->GetComponent<dae::TextComponent>();
	m_TargetSubject = &m_TargetHealth->GetSubject();
	m_TargetSubject->AddObserver(this);
	UpdateText();
}



void galaga::DisplayLivesComponent::Notify(dae::Event event, dae::GameObject*)
{
	if (event == dae::Event::PlayerDied || event == dae::Event::GameOver)
	{
		UpdateText();
	}
}

galaga::DisplayLivesComponent::~DisplayLivesComponent()
{
	if (m_TargetSubject)
	{
		m_TargetSubject->RemoveObserver(this);
	}
}

void galaga::DisplayLivesComponent::OnSubjectDestroyed(dae::Subject* subject)
{
	if (subject == m_TargetSubject)
	{
		m_TargetSubject = nullptr;
		m_TargetHealth = nullptr;
	}
}

void galaga::DisplayLivesComponent::UpdateText()
{
	if (m_TextComponent == nullptr || m_TargetHealth == nullptr)
	{
		return;
	}

	m_TextComponent->SetText(m_Label + " Lives: " + std::to_string(m_TargetHealth->GetLives()));
}