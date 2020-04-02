#pragma once

#include "widget.hpp"
#include <string>

namespace ph {

class SliderWidget : public Widget
{
public:
	SliderWidget(const char* name);

	void setIconSize(sf::Vector2f size);
	void setIconTexture(Texture*);
	void setSliderValue(float value) { mSliderValue = value; }
	void setSliderMinValue(float minValue) { mSliderMinValue = minValue; }
	void setSliderMaxValue(float maxValue) { mSliderMaxValue = maxValue; }

	float getSliderValue() const { return mSliderValue; }
	float getSliderMinValue() const { return mSliderMinValue; }
	float getSliderMaxValue() const { return mSliderMaxValue; }

private:
	void updateCurrent(float dt, unsigned char z) override;

private:
	Widget* mIconWidget;
	float mSliderValue;
	float mSliderMinValue;
	float mSliderMaxValue;
};

}
