#pragma once

#include "Component.h"

#include <string>

namespace dae
{
	class GameObject;
	class Texture2D;
	class TransformComponent;

	class RenderComponent final : public Component
	{
	public:
		explicit RenderComponent(GameObject* parent);
		RenderComponent(GameObject* parent, const std::string& filename);

		~RenderComponent() override = default;

		RenderComponent(const RenderComponent&) = delete;
		RenderComponent(RenderComponent&&) = delete;
		RenderComponent& operator=(const RenderComponent&) = delete;
		RenderComponent& operator=(RenderComponent&&) = delete;

		void Render() const override;

		void SetTexture(Texture2D* texture);
		void SetTexture(const std::string& filename);

	private:
		TransformComponent* m_pTransform{};
		Texture2D* m_pTexture{};
	};
}