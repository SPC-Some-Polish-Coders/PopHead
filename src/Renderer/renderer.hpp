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
	void init(u32 screenWidth, u32 screenHeight);
	void restart(u32 screenWidth, u32 screenHeight);
	void shutDown();
	
	void setGameWorldCamera(Camera& camera);

	void beginScene();
	void endScene();

	void submitCircle(sf::Color color, Vec2 position, float radius, u8 z, 
	                  ProjectionType = ProjectionType::gameWorld, bool isAffectedByLight = true);

	void submitQuad(Texture*, const IntRect* textureRect, const sf::Color*, const Shader* shader, Vec2 position,
	                Vec2 size, u8 z, float rotation, Vec2 rotationOrigin, ProjectionType = ProjectionType::gameWorld,
					bool isAffectedByLight = true);

	void submitBunchOfQuadsWithTheSameTexture(std::vector<QuadData>&, Texture*, const Shader*,
	                                          u8 z, ProjectionType = ProjectionType::gameWorld);

	void setChunksTexture(const Texture& texture);

	u32 registerNewChunk(const FloatRect& bounds);

	void submitChunk(std::vector<ChunkQuadData>& quadsData, const FloatRect& bounds,
	                 u8 z, u32* rendererID, sf::Color color);

	void submitGroundChunk(Vec2 pos, const FloatRect& textureRect, u8 z, sf::Color color); 

	void submitLine(sf::Color, Vec2 positionA, Vec2 positionB, float thickness = 1.f);

	void submitLine(sf::Color colorA, sf::Color colorB, Vec2 positionA, Vec2 positionB, float thickness = 1.f);

	void submitPoint(Vec2 position, sf::Color, u8 z, float size = 1.f);

	void submitLight(sf::Color color, Vec2 position, float startAngle, float endAngle,
	                 float attenuationAddition, float attenuationFactor, float attenuationSquareFactor);

	void submitLightWall(FloatRect wall);
	void submitBunchOfLightWalls(const std::vector<FloatRect>& walls);

	u32 getNrOfLights();

	void submitText(const char* text, const char* fontFilename, Vec2 position, float characterSize,
		            sf::Color textColor, u8 z, ProjectionType, bool isAffectedByLight = false);

	void submitTextWorldHD(const char* text, const char* fontFilename, Vec2 worldPos, float characterSize,
		                   sf::Color textColor, u8 z);

	void submitTextArea(const char* text, const char* fontFilename, Vec2 position, float textAreaWidth,
                        TextAligment, float size, sf::Color, u8 z, ProjectionType, bool isAffectedByLight = false);

	void setAmbientLightColor(sf::Color);

	void handleEvent(sf::Event);
}

}
