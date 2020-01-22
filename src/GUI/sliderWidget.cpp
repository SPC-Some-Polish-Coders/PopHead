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
	icon->setCenterPosition({mSliderValue / mSliderMaxValue, 0.5f});
	mIconWidget = addChildWidget(icon);
}

void SliderWidget::setIconSize(sf::Vector2f size)
{
	mIconWidget->setSize(size);
	mIconWidget->setCenterPosition({mSliderValue / mSliderMaxValue, 0.5f});
}

void SliderWidget::setIconTexture(const Texture* texture)
{
	mIconWidget->setTexture(texture);
}

void SliderWidget::updateCurrent(float dt, unsigned char z)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) 
	{
		auto mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*sWindow));
		if (mIsIconChecked || isMouseOnSliderIcon(mousePos)) {
			const float barLeftEndPos = getScreenPosition().x;
			const float barWidth = getScreenSize().x;
			if(mousePos.x <= barLeftEndPos) {
				mSliderValue = 0.f;
				mIconWidget->setCenterPosition({0.f, 0.5f});
			}
			else if(mousePos.x >= barLeftEndPos + barWidth) {
				mSliderValue = mSliderMaxValue;
				mIconWidget->setCenterPosition({1.f, 0.5f});
			}
			else {
				const float distanceFromLeftEnd = mousePos.x - barLeftEndPos;
				const float normalizedValue = distanceFromLeftEnd / barWidth;
				mSliderValue = normalizedValue * mSliderMaxValue;
				mIconWidget->setCenterPosition({mSliderValue / mSliderMaxValue, 0.5f});
			}
		}
	}
	else {
		mIsIconChecked = false;
	}
}

bool SliderWidget::isMouseOnSliderIcon(const sf::Vector2f& mousePos)
{
	return mIconWidget->getScreenPosition().x < mousePos.x
		&& mIconWidget->getScreenPosition().x + mIconWidget->getScreenSize().x > mousePos.x
		&& mIconWidget->getScreenPosition().y < mousePos.y
		&& mIconWidget->getScreenPosition().y + mIconWidget->getScreenSize().y > mousePos.y;
}

}

