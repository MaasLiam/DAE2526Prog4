#pragma once
#include "Component.h"
#include <string>
#include <memory>
#include <SDL3/SDL_pixels.h>

namespace dae
{
	class Font;
	class Texture2D;
	class TextComponent final : public Component
	{
	public:
		TextComponent(GameObject* parent, std::string text, std::shared_ptr<Font> font, SDL_Color color = { 255,255,255,255 });
		void Update(float) override;
		void Render() const override;

		void SetText(const std::string& text);
		void SetColor(const SDL_Color& color);

		
		virtual ~TextComponent() = default;
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;
	private:
		bool m_needsUpdate{};
		std::string m_text{};
		SDL_Color m_color{ 255, 255, 255, 255 };
		std::shared_ptr<Font> m_font{};
		std::shared_ptr<Texture2D> m_textTexture{};
	};
}
