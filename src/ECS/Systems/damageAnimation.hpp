#pragma once

#include "ECS/system.hpp"

namespace ph::system {

class DamageAnimation : public System
{
public:
	using System::System;

	void update(float seconds) override;
};

}
