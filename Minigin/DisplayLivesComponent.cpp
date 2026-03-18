#include "DisplayLivesComponent.h"
#include "HealthComponent.h"
#include "TextComponent.h"
#include "Event.h"
#include "GameObject.h"

namespace dae
{
	DisplayLivesComponent::DisplayLivesComponent(GameObject* owner, HealthComponent& targetHealth, std::string label)
		: Component(owner)
		, m_TargetHealth(&targetHealth)
		, m_Label(std::move(label))
	{

		m_TextComponent = GetOwner()->GetComponent<TextComponent>();
		m_TargetHealth->GetSubject().AddObserver(this);
		UpdateText();
	}

	

	void DisplayLivesComponent::Notify(Event event, GameObject*)
	{
		if (event == Event::PlayerDied || event == Event::GameOver)
		{
			UpdateText();
		}
	}

	void DisplayLivesComponent::UpdateText()
	{
		if (m_TextComponent ==  nullptr || m_TargetHealth == nullptr)
		{
			return;
		}

		m_TextComponent->SetText(m_Label + " Lives: " + std::to_string(m_TargetHealth->GetLives()));
	}

}