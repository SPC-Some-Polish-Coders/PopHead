#pragma once

#include "ECS/system.hpp"

namespace ph::system {

	class SpritesSync : public System
	{
	public:
		using System::System;

		void update(float seconds) override;
	};
}
