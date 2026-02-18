#pragma once
#include "Component.h"
#include <memory>
#include <string>

namespace dae
{
	class GameObject;
	class Texture2D;

	class RenderComponent final : public Component
	{
	public:
		RenderComponent(GameObject* parent, const std::string& fileName);
		void Render() const override;
	private:
		std::shared_ptr<Texture2D> m_Texture;
	};

}