#pragma once

#include "ECS/system.hpp"

#include "Utilities/rect.hpp"

namespace ph::system {

class VelocityChangingAreas : public System
{
public:
	using System::System;

	void update(float seconds) override;

private:
	std::vector<float> getNewMultipliers(const std::vector<FloatRect>& currentAreas) const;
	void removeVelocityChangingEffects() const;

};

}
