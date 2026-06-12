#include "TextComponent.h"

#include "Font.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "Renderer.h"
#include "Texture2D.h"

#include <stdexcept>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>

dae::TextComponent::TextComponent(GameObject* parent, std::string text, std::shared_ptr<Font> font, SDL_Color color)
	: Component(parent)
	, m_RenderComponent(parent->GetComponent<RenderComponent>())
	, m_Text(std::move(text))
	, m_Color(color)
	, m_Font(std::move(font))
{}

void dae::TextComponent::Update(float)
{
	if (!m_NeedsUpdate)
	{
		return;
	}

	if (m_RenderComponent == nullptr)
	{
		return;
	}

	if (m_Text.empty())
	{
		m_TextTexture.reset();
		m_RenderComponent->SetTexture(nullptr);
		m_NeedsUpdate = false;
		return;
	}

	const auto surface = TTF_RenderText_Blended(
		m_Font->GetFont(),
		m_Text.c_str(),
		m_Text.length(),
		m_Color
	);

	if (surface == nullptr)
	{
		throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
	}

	auto* texture = SDL_CreateTextureFromSurface(
		Renderer::GetInstance().GetSDLRenderer(),
		surface
	);

	SDL_DestroySurface(surface);

	if (texture == nullptr)
	{
		throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
	}

	m_TextTexture = std::make_shared<Texture2D>(texture);
	m_RenderComponent->SetTexture(m_TextTexture.get());

	m_NeedsUpdate = false;
}

void dae::TextComponent::SetText(const std::string& text)
{
	if (m_Text == text)
	{
		return;
	}

	m_Text = text;
	m_NeedsUpdate = true;
}

void dae::TextComponent::SetColor(const SDL_Color& color)
{
	m_Color = color;
	m_NeedsUpdate = true;
}