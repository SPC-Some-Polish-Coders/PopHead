#pragma once

#include <string>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

namespace ph {

void initTextRenderer();
void drawFontBitmap(const char* filename, float size);
void drawText(const char* text, const char* filename, sf::Vector2f position, float size, sf::Color color);

}

