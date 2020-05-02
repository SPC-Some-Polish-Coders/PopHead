#pragma once

#include "ECS/system.hpp"

namespace ph::system {

	class SlowZombieSystem : public System
	{
	public:
		using System::System;

		void update(float dt) override;
	};
}
