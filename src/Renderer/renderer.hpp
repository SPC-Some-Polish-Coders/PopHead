#pragma once

#include "Vertices/vertexArray.hpp"
#include "Shaders/shaderLibary.hpp"
#include "texture.hpp"
#include "camera.hpp"
#include "glEnums.hpp"
#include "MinorRenderers/SFMLrenderer.hpp"
#include "Utilities/rect.hpp"
#include <SFML/System/Vector2.hpp>
#include <optional>

namespace ph {

class EfficiencyRegister;

class Renderer
{
public:
	static void init(unsigned screenWidth, unsigned screenHeight);
	static void reset(unsigned screenWidth, unsigned screenHeight);
	static void shutDown();
	
	static void beginScene(Camera&);
	static void endScene(sf::RenderWindow& window, EfficiencyRegister&);

	static void slowSubmitQuad(const Texture*, const IntRect* textureRect, const sf::Color*, const Shader*,
	                       sf::Vector2f position, sf::Vector2i size, float rotation = 0.f);

	static void submitQuad(const Texture*, const IntRect* textureRect, const sf::Color*,
	                       sf::Vector2f position, sf::Vector2f size, float rotation = 0.f);

	static void submit(const sf::Drawable&);

	static void onWindowResize(unsigned width, unsigned height);

	static void setClearColor(const sf::Color&);

private:
	static void flushInstancedSprites();
	static auto getTextureSlotToWhichThisTextureIsBound(const Texture*) -> std::optional<int>;
	static auto getNormalizedTextureRect(const IntRect* pixelTextureRect, sf::Vector2f size) -> FloatRect;
	static bool isInsideScreen(sf::Vector2f position, sf::Vector2f size);
	static bool isInsideScreen(const FloatRect objectBounds);
};

}
