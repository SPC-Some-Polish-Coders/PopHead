#pragma once

#include "widget.hpp"
#include <SFML/Graphics/Color.hpp>

namespace ph {

class TextWidget : public Widget 
{
public:
	TextWidget(const char* name);

	void updateCurrent(float dt, float z) override;

	void setString(const char* text);
	void setTextColor(sf::Color color) { mTextColor = color; }
	void setTextSize(float size) { mTextSize = size; }
	void setFontName(const char* fontName);
	void setScrollingEffect(bool se) { mScrollingEffect = se; }

private:
	char mFontName[50];
	std::string mText;
	float mTextSize;
	sf::Color mTextColor;
	bool mScrollingEffect;
};

}

