#pragma once

#include "ECS/system.hpp"

namespace ph::system {

class PatricleSystem : public System
{
public:
	using System::System;

	void update(float seconds) override;
};

}