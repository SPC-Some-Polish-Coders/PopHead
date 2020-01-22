#pragma once

#include "widget.hpp"
#include "Renderer/API/textAligment.hpp"
#include <SFML/Graphics/Color.hpp>

namespace ph {

class TextWidget : public Widget 
{
public:
	TextWidget(const char* name);

	void updateCurrent(float dt, unsigned char z) override;

	void setText(const char* text);
	void setText(const std::string& text) { mText = text; }
	void setTextAligment(TextAligment aligment) { mTextAligment = aligment; }
	void setTextColor(sf::Color color) { mTextColor = color; }
	void setFontSize(float size) { mFontSize = size; }
	void setFontName(const char* fontName);
	void setScrollingEffect(bool se) { mScrollingEffect = se; }

private:
	char mFontName[50];
	std::string mText;
	TextAligment mTextAligment;
	float mFontSize;
	sf::Color mTextColor;
	bool mScrollingEffect;
};

}

