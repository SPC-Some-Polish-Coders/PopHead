#pragma once

#include "Utilities/rect.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

namespace sf {
	class Drawable;
	class RenderWindow;
}

namespace ph {

class DebugCounter;
class Camera;
class Texture;
class Shader;

namespace Renderer
{
	void init(unsigned screenWidth, unsigned screenHeight);
	void restart(unsigned screenWidth, unsigned screenHeight);
	void shutDown();
	
	void beginScene(Camera&);
	void endScene(sf::RenderWindow& window, DebugCounter&);

	void submitQuad(const Texture*, const IntRect* textureRect, const sf::Color*, const Shader* shader,
	                sf::Vector2f position, sf::Vector2f size, unsigned char z, float rotation = 0.f);

	void submitLine(const sf::Color&, const sf::Vector2f positionA, const sf::Vector2f positionB, float thickness = 1.f);

	void submitLine(const sf::Color& colorA, const sf::Color& colorB,
	                const sf::Vector2f positionA, const sf::Vector2f positionB, float thickness = 1.f);

	void submitPoint(sf::Vector2f position, const sf::Color&, unsigned char z, float size = 1.f);

	void submitSFMLObject(const sf::Drawable&);

	void onWindowResize(unsigned width, unsigned height);

	void setClearColor(const sf::Color&);
};

}
