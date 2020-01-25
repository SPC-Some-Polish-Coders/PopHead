#pragma once

#include "ECS/system.hpp"

namespace ph {

namespace system {

	class HintAreas : public System
	{
	public:
		using System::System;

		void update(float dt) override;
	};

}}
