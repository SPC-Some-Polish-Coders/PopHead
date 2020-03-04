#include "sliderWidget.hpp"
#include "Renderer/renderer.hpp"
#include <cmath>

namespace ph {

SliderWidget::SliderWidget(const char* name)
	:Widget(name)
	,mSliderValue(50.f)
	,mSliderMaxValue(100.f)
{
	auto* icon = new Widget("sliderIcon");
	icon->setSize({0.15f, 1.f});
	mIconWidget = addChildWidget(icon);
}

void SliderWidget::setIconSize(sf::Vector2f size)
{
	mIconWidget->setSize(size);
}

void SliderWidget::setIconTexture(Texture* texture)
{
	mIconWidget->setTexture(texture);
}

void SliderWidget::updateCurrent(float dt, unsigned char z)
{
	mIconWidget->setCenterPosition({mSliderValue / (mSliderMaxValue - mSliderMinValue), 0.5f});

	if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) 
	{
		auto mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*sWindow));
		const sf::Vector2f barScreenPos = getScreenPosition();
		const sf::Vector2f barScreenSize = getScreenSize();
		bool isMouseOnSlider = FloatRect(barScreenPos, barScreenSize).contains(mousePos);
		if(isMouseOnSlider) 
		{
			if(mousePos.x <= barScreenPos.x) {
				mSliderValue = mSliderMinValue;
			}
			else if(mousePos.x >= barScreenPos.x + getScreenSize().x) {
				mSliderValue = mSliderMaxValue;
			}
			else {
				const float normalizedValue = (mousePos.x - barScreenPos.x) / getScreenSize().x;
				mSliderValue = mSliderMinValue + (normalizedValue * (mSliderMaxValue - mSliderMinValue));
			}
		}
	}
}

}

