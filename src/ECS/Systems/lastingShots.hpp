#pragma once

#include "ECS/system.hpp"
#include "Utilities/rect.hpp"

#include <SFML/System/Vector2.hpp>

namespace ph::system {

	class LastingShots : public System
	{
	public:
		using System::System;

		void update(float seconds) override;
	};
}
