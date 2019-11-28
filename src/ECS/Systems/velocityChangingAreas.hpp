#pragma once

#include "ECS/system.hpp"

#include <set>
#include "Utilities/rect.hpp"

namespace ph::system {

class VelocityChangingAreas : public System
{
public:
	using System::System;

	void update(float seconds) override;

private:
	std::multiset<float> getAllNewMultipliers(const std::vector<FloatRect>& currentAreas) const;
	std::multiset<float> getActualNewMultipliers(const std::multiset<float>& minuend, const std::multiset<float>& subtrahend) const;
	void removeVelocityEffectsFromObjectsBeyond() const;

};

}
