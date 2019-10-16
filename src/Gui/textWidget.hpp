#pragma once

#include "widget.hpp"

namespace ph {

class TextWidget : public Widget 
{
public:
	TextWidget();

	void setString(const std::string& text);

	void setColor(const sf::Color& color);

	void setTextPosition(const sf::Vector2f& pos);

	void setAlpha(unsigned int alpha) override;

	void setTextOrigin(const sf::Vector2f& origin);

	void setTextAlpha(unsigned int alpha);

	void scaleText(const sf::Vector2f& scale);

	void setPosition(const sf::Vector2f& pos) override;

	void move(const sf::Vector2f& delta) override;

	void setCharacterSize(unsigned int size);

	void setFontPath(const std::string& path);

	void scale(const sf::Vector2f& scale) override;

	void draw() override;

	void setScrollingEffect(bool flag);
	static void resetAnimation();


private:
	sf::Text mText;
	sf::Vector2f mTextPosition;
	bool scrollingEffect = false;
	static float sec;
};

}
