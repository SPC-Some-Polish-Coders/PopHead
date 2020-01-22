#pragma once

#include "Renderer/API/textAligment.hpp"
#include "quadData.hpp"
#include "Renderer/API/font.hpp"
#include "Renderer/API/shader.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>

namespace ph {

class TextRenderer
{
public:
	void init();
	void shutDown();

	void beginDebugDisplay();

	void drawText(const char* text, const char* fontFilename, sf::Vector2f position, float fontSize, sf::Color, ProjectionType);
	void drawDebugText(const char* text, const char* fontFilename, float fontSize, float upMargin, float downMargin, sf::Color);
	void drawTextArea(const char* text, const char* fontFilename, sf::Vector2f position, const float textAreaWidth,
                      TextAligment, float fontSize, sf::Color textColor, ProjectionType);
private:
	void drawTextInternal(const char* text, const char* fontFilename, sf::Vector2f position, float fontSize, sf::Color, ProjectionType);

private:
	FontHolder mFontHolder;
	Shader mTextShader;
	sf::Vector2f mDebugTextPosition;
	bool mWasDebugTextDrawnInLastFrame;
};

}

