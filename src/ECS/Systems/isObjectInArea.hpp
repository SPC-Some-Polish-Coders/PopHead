#pragma once

#include "ECS/system.hpp"

#include "Utilities/rect.hpp"

namespace ph::system {

class IsObjectInArea : public System
{
public:
	using System::System;

	void update(float seconds) override;

//private:
//	void handleObjectsOutsideAreas() const;
//	void handleObjectsInsideAreas() const;
//	auto positionInVector(const std::vector<FloatRect>& sizes, const FloatRect& sizeValue) const -> std::vector<FloatRect>::const_iterator;

};

}
