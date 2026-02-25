#include <stdexcept>
#include <SDL3_ttf/SDL_ttf.h>
#include "TextComponent.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"

#include "GameObject.h"
#include "TransformComponent.h"
#include "RenderComponent.h"

dae::TextComponent::TextComponent(GameObject& parent, std::string text, std::shared_ptr<Font> font, SDL_Color color)
	:Component(parent), 
	m_needsUpdate(true), 
	m_text(text), 
	m_color(color), 
	m_font(std::move(font)), 
	m_textTexture(nullptr)
{ 
}


void dae::TextComponent::Update(float)
{
	if (m_needsUpdate)
	{
		auto* render = GetOwner().GetComponent<RenderComponent>();
		if (render)
		{
			const auto surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), m_text.length(), m_color);
			if (surf == nullptr)
			{
				throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
			}
			auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
			if (texture == nullptr)
			{
				throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
			}
			SDL_DestroySurface(surf);
			m_textTexture = std::make_shared<Texture2D>(texture);
			render->SetTexture(m_textTexture.get());
			m_needsUpdate = false;
		}
		
	}
}

void dae::TextComponent::SetText(const std::string& text)
{
	m_text = text;
	m_needsUpdate = true;
}

void dae::TextComponent::SetColor(const SDL_Color& color)
{ 
	m_color = color; 
	m_needsUpdate = true; 
}


