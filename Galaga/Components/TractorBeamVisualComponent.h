#pragma once

#include "Component.h"

class TractorBeamVisualComponent final : public dae::Component
{
public:
	explicit TractorBeamVisualComponent(dae::GameObject* owner)
		: dae::Component(owner)
	{}
};