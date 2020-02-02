#pragma once

#include "ECS/system.hpp"

namespace ph {

namespace system {

class GameplayUI : public System
{
public:
	using System::System;

	void update(float dt) override;
};

}}
