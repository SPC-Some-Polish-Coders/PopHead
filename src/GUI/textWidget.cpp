#include "textWidget.hpp"
#include "Renderer/renderer.hpp"
#include <cstring>

namespace ph {

TextWidget::TextWidget(const char* name)
	:Widget(name)
	,mFontName()
	,mTextAligment(TextAligment::center)
	,mFontSize(30.f)
	,mTextColor(sf::Color::White)
{
}

void TextWidget::updateCurrent(float dt, unsigned char z)
{
	for(const auto& behaviour : mBehaviors)
		if(behaviour.first == BehaviorType::onUpdate)
			behaviour.second(this);

	Renderer::submitTextArea(mText.c_str(), mFontName, getScreenPosition(), getScreenSize().x,
		mTextAligment, mFontSize, mTextColor, --z, ProjectionType::gui, false);
}

void TextWidget::setText(const char* text)
{
	mText = text;
}

void TextWidget::setFontName(const char* fontName)
{
	std::strcpy(mFontName, fontName);
}

}
