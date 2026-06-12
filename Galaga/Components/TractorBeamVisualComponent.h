#pragma once

#include "Component.h"

namespace galaga
{
	class TractorBeamVisualComponent final : public dae::Component
	{
	public:
		explicit TractorBeamVisualComponent(dae::GameObject* owner)
			: dae::Component(owner)
		{
		
		}

		~TractorBeamVisualComponent() override = default;

		TractorBeamVisualComponent(const TractorBeamVisualComponent&) = delete;
		TractorBeamVisualComponent(TractorBeamVisualComponent&&) = delete;
		TractorBeamVisualComponent& operator=(const TractorBeamVisualComponent&) = delete;
		TractorBeamVisualComponent& operator=(TractorBeamVisualComponent&&) = delete;
	};
}