#pragma once

#include "ECS/system.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace ph::system {

	class SpritesSync : public System
	{
	public:
		using System::System;

		void update(float seconds) override;
	};
}
