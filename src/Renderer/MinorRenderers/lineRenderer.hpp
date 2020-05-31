#pragma once

#include "Utilities/rect.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

namespace ph::LineRenderer {

	void init();
	void shutDown();

	void submitLine(sf::Color colorA, sf::Color colorB, Vec2 positionA, Vec2 positionB, float thickness = 1.f);

	void flush(); 

	void submitDebug();

	void setScreenBoundsPtr(const FloatRect* screenBounds);

}
