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

		~FPSComponent() override = default;

		FPSComponent(const FPSComponent&) = delete;
		FPSComponent(FPSComponent&&) = delete;
		FPSComponent& operator=(const FPSComponent&) = delete;
		FPSComponent& operator=(FPSComponent&&) = delete;

		private:
			TextComponent* m_pTextComponent{};
			float m_AccumulatedTime{};
			int m_FrameCount{};
			const float m_UpdateInterval{};
	};
}