#pragma once
#include "Component.h"

namespace dae
{
	class TextComponent;
	class GameObject;

	class FPSComponent final : public Component
	{
	public:
		explicit FPSComponent(GameObject* parent);

		~FPSComponent() override = default;

		FPSComponent(const FPSComponent&) = delete;
		FPSComponent(FPSComponent&&) = delete;
		FPSComponent& operator=(const FPSComponent&) = delete;
		FPSComponent& operator=(FPSComponent&&) = delete;

		void Update(float deltaTime) override;

	private:
		static constexpr float UpdateInterval{ 0.1f };

		TextComponent* m_TextComponent{};
		float m_AccumulatedTime{};
		int m_FrameCount{};
	};
}