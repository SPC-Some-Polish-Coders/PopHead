#pragma once

#include "widget.hpp"
#include <string>

namespace ph {

class SliderWidget : public Widget
{
public:
	SliderWidget(const char* name, const std::string& iconTexturePath, float initSliderValue, float sliderMaxValue);

	void setIconSize(sf::Vector2f size);
	float getSliderValue() const { return mSliderValue; }
	float getSliderMaxValue() const { return mSliderMaxValue; }

private:
	void updateCurrent(float dt, float z) override;
	bool isMouseOnSliderIcon(const sf::Vector2f& distanceFromCenter);

private:
	Widget* mIconWidget;
	float mSliderValue;
	const float mSliderMaxValue;
	bool mIsIconChecked = false;
};

}
