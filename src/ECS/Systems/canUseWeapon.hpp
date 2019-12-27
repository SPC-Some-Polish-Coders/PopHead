#pragma once

#include "ECS/system.hpp"

namespace ph::system {

	class CanUseWeapon : public System
	{
	public:
		using System::System;

		void update(float dt) override;
	};

}
