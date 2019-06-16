#include "Widget.hpp"

#ifndef TEXT_WIDGET
#define TEXT_WIDGET

namespace ph {

	class TextWidget : public Widget {

	public:
		TextWidget();

		void setString(const std::string& text);

		void setColor(const sf::Color& color);

		void setTextPosition(const sf::Vector2f& pos);

		void setAlpha(unsigned int alpha);

		void setTextOrigin(const sf::Vector2f& origin);

		void setTextAlpha(unsigned int alpha);

		void scaleText(const sf::Vector2f& scale);

		void setPosition(const sf::Vector2f& pos);

		void move(const sf::Vector2f& delta);

		void setCharacterSize(unsigned int size);

		void setFontPath(const std::string& path);

		void scale(const sf::Vector2f& scale);

		void draw();

	private:
		sf::Text mText;
		sf::Vector2f mTextPosition;
		
	};


}

#endif