#pragma once

#include "widget.hpp"
#include <SFML/Graphics/Color.hpp>

namespace ph {

class TextWidget : public Widget 
{
public:
	TextWidget();

	void setString(const std::string& text);
	void setTextColor(sf::Color color) { mTextColor = color; }
	void setTextPosition(const sf::Vector2f& pos);
	void setTextOrigin(const sf::Vector2f& origin);
	void setTextAlpha(unsigned int alpha);
	void scaleText(const sf::Vector2f& scale);
	void setCharacterSize(unsigned int size);
	void setFontPath(const std::string& path);
	void setScrollingEffect(bool flag);

private:
	std::string mText;
	sf::Vector2f mTextPosition;
	sf::Color mTextColor;
	bool scrollingEffect = false;
};

}

