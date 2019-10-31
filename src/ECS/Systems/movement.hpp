#pragma once

#include "ECS/system.hpp"

namespace ph::system {
	
	class Movement : public System 
	{
	public:
		using System::System;

		void update(float seconds) override;
	};
}
