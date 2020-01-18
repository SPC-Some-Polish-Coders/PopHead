#pragma once

#include "widget.hpp"
#include <string>

namespace ph {

class SliderWidget : public Widget
{
public:
	void createSlider(std::string path);
	int getSliderValue();

private:
	void handleEventOnCurrent(const ph::Event&) override;
	bool isMouseOnSliderIcon(const sf::Vector2f& distanceFromCenter);

	std::unique_ptr<Widget> mIcon;
	bool mIsIconChecked = false;
};

}
