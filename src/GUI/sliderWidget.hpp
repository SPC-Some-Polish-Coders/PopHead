#pragma once

#include "widget.hpp"
#include <string>

namespace ph {

class SliderWidget : public Widget
{
public:
	SliderWidget(const char* name);

	void setIconSize(sf::Vector2f size);
	void setIconTexture(const Texture*);
	void setSliderValue(float value) { mSliderValue = value; }
	void setSliderMaxValue(float maxValue) { mSliderMaxValue = maxValue; }

	float getSliderValue() const { return mSliderValue; }
	float getSliderMaxValue() const { return mSliderMaxValue; }

private:
	void updateCurrent(float dt, unsigned char z) override;
	bool isMouseOnSliderIcon(const sf::Vector2f& distanceFromCenter);

private:
	Widget* mIconWidget;
	float mSliderValue;
	float mSliderMaxValue;
	bool mIsIconChecked = false;
};

}
