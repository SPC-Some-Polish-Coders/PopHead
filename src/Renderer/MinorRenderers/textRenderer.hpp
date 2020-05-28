#pragma once

#include "Renderer/API/textAligment.hpp"
#include "quadData.hpp"
#include "Renderer/API/font.hpp"
#include "Renderer/API/shader.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

namespace ph {
	class Camera;
}

namespace ph::TextRenderer {

void init();
void shutDown();

void drawText(const char* text, const char* fontFilename, Vec2 position,
			  float fontSize, sf::Color textColor, u8 z, ProjectionType, bool isAffectedByLight);

void drawTextArea(const char* text, const char* fontFilename, Vec2 position, float textAreaWidth,
				  TextAligment, float fontSize, sf::Color textColor, u8 z, ProjectionType, bool isAffectedByLight);

void drawTextWorldHD(const char* text, const char* fontFilename, Vec2 worldPos, 
                     const Camera& worldCam, float fontSize, sf::Color textColor, u8 z);
}

