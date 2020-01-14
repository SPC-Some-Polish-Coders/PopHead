#pragma once

#include "Renderer/API/font.hpp"
#include "Renderer/API/shader.hpp"
#include <string>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

namespace ph {

class TextRenderer
{
public:
	void init();
	void shutDown();

	void drawText(const char* text, const char* fontFilename, sf::Vector2f position, float size, sf::Color color);

private:
	FontHolder mFontHolder;
	Shader* mTextShader;
	unsigned mTextVAO, mTextVBO, mTextIBO;
};

}

