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

void drawText(const char* text, const char* fontFilename, sf::Vector2f position,
			  float fontSize, sf::Color textColor, unsigned char z, ProjectionType, bool isAffectedByLight);

void drawTextArea(const char* text, const char* fontFilename, sf::Vector2f position, float textAreaWidth,
				  TextAligment, float fontSize, sf::Color textColor, unsigned char z, ProjectionType, bool isAffectedByLight);

void drawTextWorldHD(const char* text, const char* fontFilename, sf::Vector2f worldPos, 
                     const Camera& worldCam, float fontSize, sf::Color textColor, unsigned char z);
}

