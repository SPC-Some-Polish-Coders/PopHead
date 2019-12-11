#pragma once

#include "ECS/system.hpp"

namespace ph::system {

	class CanUseGun : public System
	{
	public:
		using System::System;

		void update(float seconds) override;
	};

	class CanUseMelee : public System
	{
	public:
		using System::System;

		void update(float seconds) override;
	};

}
