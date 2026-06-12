#pragma once

#include "Component.h"

#include <memory>
#include <string>

#include <SDL3/SDL_pixels.h>

namespace dae
{
	class Font;
	class GameObject;
	class RenderComponent;
	class Texture2D;

	class TextComponent final : public Component
	{
	public:
		TextComponent(GameObject* parent, std::string text, std::shared_ptr<Font> font, SDL_Color color = { 255, 255, 255, 255 });
		~TextComponent() override = default;

		TextComponent(const TextComponent&) = delete;
		TextComponent(TextComponent&&) = delete;
		TextComponent& operator=(const TextComponent&) = delete;
		TextComponent& operator=(TextComponent&&) = delete;

		void Update(float deltaTime) override;

		void SetText(const std::string& text);
		void SetColor(const SDL_Color& color);

	private:
		RenderComponent* m_RenderComponent{};
		bool m_NeedsUpdate{ true };
		std::string m_Text{};
		SDL_Color m_Color{ 255, 255, 255, 255 };
		std::shared_ptr<Font> m_Font{};
		std::shared_ptr<Texture2D> m_TextTexture{};
	};
}