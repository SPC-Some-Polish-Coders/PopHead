#pragma once

#include "ECS/system.hpp"

#include <SFML/System/Vector2.hpp>

namespace ph::system {

	class PlayerMovementInput : public System
	{
	public:
		using System::System;

		void update(float seconds) override;

	private:
		sf::Vector2f getPlayerDirection() const;
	};
}
