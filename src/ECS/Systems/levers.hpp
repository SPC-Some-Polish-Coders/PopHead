#pragma once

#include "ECS/system.hpp"

#include "Utilities/rect.hpp"

namespace ph::system {

class Levers : public System
{
public:
	using System::System;

	void update(float seconds) override;

private:
	void handleListeners(bool isActivated, const sf::Vector2f& leverPosition) const;

};

}