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
        explicit RenderComponent(GameObject& parent);

        RenderComponent(GameObject& parent, const std::string& filename);

        void Render() const override;

        void SetTexture(Texture2D* texture);

    private:
		Texture2D* m_pTexture{};
    };

}