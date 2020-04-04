#pragma once

#include "API/textAligment.hpp"
#include "MinorRenderers/quadData.hpp"
#include "Utilities/rect.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Event.hpp>
#include <vector>

namespace ph {

class Camera;
class Texture;
class Shader;

namespace Renderer
{
	void init(unsigned screenWidth, unsigned screenHeight);
	void restart(unsigned screenWidth, unsigned screenHeight);
	void shutDown();
	
	void setGameWorldCamera(Camera& camera);

	void beginScene();
	void endScene();

	void submitQuad(Texture*, const IntRect* textureRect, const sf::Color*, const Shader* shader, sf::Vector2f position,
	                sf::Vector2f size, unsigned char z, float rotation, sf::Vector2f rotationOrigin, ProjectionType = ProjectionType::gameWorld,
					bool isAffectedByLight = true);

	void submitBunchOfQuadsWithTheSameTexture(std::vector<QuadData>&, Texture*, const Shader*,
	                                          unsigned char z, ProjectionType = ProjectionType::gameWorld);

	void submitChunk(std::vector<ChunkQuadData>& quadsData, const Texture& texture,
	                 const FloatRect& bounds, unsigned char z, unsigned* rendererID);

	void submitGroundChunk(sf::Vector2f pos, const Texture& texture, const FloatRect& textureRect, unsigned char z); 

	void submitLine(sf::Color, const sf::Vector2f positionA, const sf::Vector2f positionB, float thickness = 1.f);

	void submitLine(sf::Color colorA, sf::Color colorB, const sf::Vector2f positionA, const sf::Vector2f positionB, float thickness = 1.f);

	void submitPoint(sf::Vector2f position, sf::Color, unsigned char z, float size = 1.f);

	void submitLight(sf::Color color, sf::Vector2f position, float startAngle, float endAngle,
	                 float attenuationAddition, float attenuationFactor, float attenuationSquareFactor);

	void submitLightWall(FloatRect wall);
	void submitBunchOfLightWalls(const std::vector<FloatRect>& walls);

	unsigned getNrOfLights();

	void submitText(const char* text, const char* fontFilename, sf::Vector2f position, float characterSize,
		            sf::Color textColor, unsigned char z, ProjectionType, bool isAffectedByLight = false);

	void submitDebugText(const char* text, const char* fontFilename, float characterSize, float upMargin, float downMargin,
                         sf::Color textColor);

	void submitTextArea(const char* text, const char* fontFilename, sf::Vector2f position, float textAreaWidth,
                        TextAligment, float size, sf::Color, unsigned char z, ProjectionType, bool isAffectedByLight = false);

	void setAmbientLightColor(sf::Color);

	void handleEvent(sf::Event);
}

}
