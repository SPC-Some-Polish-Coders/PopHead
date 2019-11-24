#pragma once

#include "ECS/system.hpp"

namespace ph::system {

	class VelocityClear : public System
	{
	public:
		using System::System;

		void update(float seconds) override;
	};
}
