#pragma once

#include <string>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

namespace ph {

void initTextRenderer();
void drawFontBitmap();
void drawText(const char* text, sf::Vector2f position, float size, sf::Color color);

}

