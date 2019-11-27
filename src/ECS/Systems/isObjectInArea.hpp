#pragma once

#include "ECS/system.hpp"
#include "Utilities/rect.hpp"

namespace ph::system {

class IsObjectInArea : public System
{
public:
	using System::System;

	void update(float seconds) override;

private:
	bool isSizeInVector(const std::vector<FloatRect>& sizes, const FloatRect& sizeValue) const;

};

}
