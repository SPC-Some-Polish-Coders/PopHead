#pragma once

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

	void drawText(const char* text, const char* fontFilename, sf::Vector2f position, float size, sf::Color);
	void drawDebugText(const char* text, const char* fontFilename, float size, float upMargin, float downMargin, sf::Color color);
private:
	void drawTextInternal(Shader&, const char* text, const char* fontFilename, sf::Vector2f position, float size, sf::Color color);

private:
	FontHolder mFontHolder;
	Shader mTextShader;
	Shader mDebugTextShader;
	Shader mDebugTextBackgroundShader;
	sf::Vector2f mDebugTextPosition;
	unsigned mTextVAO, mTextVBO;
	unsigned mDebugTextBackgroundVAO, mDebugTextBackgroundVBO;
	unsigned mIBO;
	bool mWasDebugTextDrawnInLastFrame;
};

}

