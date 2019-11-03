#pragma once

#include "ECS/system.hpp"

namespace ph::system {

	class PickupMedkit : public System
	{
	public:
		using System::System;

		void update(float seconds) override;
	};

	class PickupBullet : public System
	{
	public:
		using System::System;

		void update(float seconds) override;

	
	};
}
