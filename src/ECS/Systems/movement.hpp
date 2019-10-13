#pragma once

#include "ECS/system.hpp"

#include <iostream>

namespace ph::system {
	
	class Movement : public System 
	{
	public:
		using System::System;

		void update(float seconds) override;
	};
}
