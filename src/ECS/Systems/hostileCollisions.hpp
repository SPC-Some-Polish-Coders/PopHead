#pragma once

#include "ECS/system.hpp"

namespace ph::system {

	class HostileCollisions : public System
	{
	public:
		using System::System;

		void update(float dt) override;

		inline static bool godMode = false;
	};
}
