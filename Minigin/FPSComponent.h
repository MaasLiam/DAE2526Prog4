#pragma once
#include "Component.h"

namespace dae
{
	class TextComponent;
	class GameObject;
	class FPSComponent final : public Component
	{
	public:
		FPSComponent(GameObject* parent); 
		void Update(float deltaTime);

		private:
			TextComponent* m_pTextComponent{};
			float m_AccumulatedTime{};
			int m_FrameCount{};
			const float m_UpdateInterval{};
	};
}