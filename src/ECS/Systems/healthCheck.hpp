#pragma once

#include "ECS/system.hpp"

namespace ph::system {

	class HealthCheck : public System
	{
	public:
		using System::System;

		void update(float seconds) override;
	};
}