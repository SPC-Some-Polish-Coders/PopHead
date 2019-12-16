#pragma once

#include "ECS/system.hpp"

#include "Utilities/rect.hpp"

namespace ph::system {

class IsObjectInArea : public System
{
public:
	using System::System;

	void update(float dt) override;
};

}
