#include "sliderWidget.hpp"
#include "gameData.hpp"

#include <cmath>

namespace ph {
namespace {
	float getNewIconXPosition(double distanceXFromCenter, double parentXSize, int iconSize) {
		double x = (parentXSize / 2) + distanceXFromCenter;
		if (x > parentXSize)
			return 0.95f;
		if (x < 0)
			return 0.f;
		return static_cast<float>(std::min((x - iconSize * 0.5) / parentXSize, 0.95));
	}
}
SliderWidget::SliderWidget()
{
}
int SliderWidget::getSliderValue() {
	return static_cast<int>(icon->getPosition().x * 100);
}


void SliderWidget::createSlider(std::string path) {
	icon = std::make_unique<Widget>();
	addWidget("slider", icon.get());
	icon->setContentPath(path);
	icon->setPosition({ 0.95f, 0.f });
}

void SliderWidget::handleEventOnCurrent(const ph::Event& phEvent)
{
	if (auto* e = std::get_if<sf::Event>(&phEvent))
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			auto c = sf::Mouse::getPosition(mGameData->getWindow());
			auto distanceFromCenter = mWindow->mapPixelToCoords(c);
			if (isIconChecked || isMouseOnSliderIcon(distanceFromCenter))
			{
				isIconChecked = true;
				icon->setPosition({ getNewIconXPosition(distanceFromCenter.x, mSize.x, icon->getSize().x), 0});
			}
			for (const auto& k : mBehaviors)
				if (k.first == BehaviorType::onPressed)
					k.second(this);
		}
		else isIconChecked = false;
	}
}

bool SliderWidget::isMouseOnSliderIcon(const sf::Vector2f& distanceFromCenter)
{
	return icon->getGlobalPosition().x < distanceFromCenter.x
		&& icon->getGlobalPosition().x + icon->getSize().x > distanceFromCenter.x
		&& icon->getGlobalPosition().y < distanceFromCenter.y
		&& icon->getGlobalPosition().y + icon->getSize().y > distanceFromCenter.y;
}
}

