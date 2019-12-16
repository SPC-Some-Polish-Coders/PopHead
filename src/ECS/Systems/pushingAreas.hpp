#pragma once

#include "ECS/system.hpp"

namespace ph::system {

class PushingAreas : public System
{
public:
	using System::System;

	void update(float seconds) override;

};

}
