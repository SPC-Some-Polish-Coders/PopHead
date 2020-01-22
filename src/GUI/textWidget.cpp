#include "textWidget.hpp"
#include "Renderer/renderer.hpp"
#include <cstring>

namespace ph {

TextWidget::TextWidget(const char* name)
	:Widget(name)
	,mFontName()
	,mTextAligment(TextAligment::center)
	,mTextSize(30.f)
	,mTextColor(sf::Color::White)
	,mScrollingEffect(false)
{
}

void TextWidget::updateCurrent(float dt, unsigned char z)
{
	if(mIsActive) 
	{
		for(const auto& behaviour : mBehaviors)
			if(behaviour.first == BehaviorType::onUpdate)
				behaviour.second(this);

		if (mScrollingEffect)
			move({0, -0.35f});

		Renderer::submitTextArea(mText.c_str(), mFontName, getScreenPosition(), getScreenSize().x,
			mTextAligment, mTextSize, mTextColor, --z, ProjectionType::gui);
	}
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
