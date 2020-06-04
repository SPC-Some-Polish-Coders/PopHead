#pragma once

#include "ECS/system.hpp"

namespace ph::system {

class SpikesSystem : public System
{
public:
	using System::System;

	void update(float dt) override;
};

}
