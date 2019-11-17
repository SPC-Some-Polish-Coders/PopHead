#pragma once

#include "Utilities/rect.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

namespace sf {
	class Drawable;
	class RenderWindow;
}

namespace ph {

class EfficiencyRegister;
class Camera;
class Texture;
class Shader;

namespace Renderer
{
	void init(unsigned screenWidth, unsigned screenHeight);
	void restart(unsigned screenWidth, unsigned screenHeight);
	void shutDown();
	
	void beginScene(Camera&);
	void endScene(sf::RenderWindow& window, EfficiencyRegister&);

	void slowSubmitQuad(const Texture*, const IntRect* textureRect, const sf::Color*, const Shader*,
	                sf::Vector2f position, sf::Vector2i size, float rotation = 0.f);

	void submitQuad(const Texture*, const IntRect* textureRect, const sf::Color*,
	                sf::Vector2f position, sf::Vector2f size, float rotation = 0.f);

	void submitSFMLObject(const sf::Drawable&);

	void onWindowResize(unsigned width, unsigned height);

	void setClearColor(const sf::Color&);
};

}
