#pragma once

#include "Utilities/rect.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

namespace ph {

class Shader;

class LineRenderer
{
public:
	void init();
	void shutDown();

	void setScreenBoundsPtr(const FloatRect* screenBounds) { mScreenBounds = screenBounds; }

	unsigned getNumberOfDrawCalls() const { return mNumberOfDrawCalls; }
	unsigned getNumberOfDrawnLines() const { return mNumberOfDrawCalls; }

	void setDebugNumbersToZero();

	void drawLine(const sf::Color& colorA, const sf::Color& colorB,
	              const sf::Vector2f positionA, const sf::Vector2f positionB, float thickness = 1.f);
private:
	Shader* mLineShader;
	const FloatRect* mScreenBounds;
	unsigned mLineVAO;
	unsigned mLineVBO;
	unsigned mNumberOfDrawCalls;
};

}
