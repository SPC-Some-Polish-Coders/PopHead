#include "sliderWidget.hpp"
#include "gameData.hpp"

#include <cmath>

namespace ph {

namespace {

	// TODO_gui
	//float getNewIconXPosition(double distanceXFromCenter, double parentXSize, int iconSize) {
	//	double x = (parentXSize / 2) + distanceXFromCenter;
	//	if (x > parentXSize)
	//		return 0.95f;
	//	if (x < 0)
	//		return 0.f;
	//	return static_cast<float>(std::min((x - iconSize * 0.5) / parentXSize, 0.95));
	//}
}

int SliderWidget::getSliderValue() {
	// TODO_gui
	return 50;
	//return static_cast<int>(mIcon->getPosition().x * 100);
}

void SliderWidget::createSlider(std::string path) 
{
	// TODO_gui
	/*mIcon = std::make_unique<Widget>();
	addChildWidget("slider", mIcon.get());
	mIcon->setContentPath(path);
	mIcon->setPosition({ 0.95f, 0.f });*/
}

void SliderWidget::handleEventOnCurrent(const ph::Event& phEvent)
{
	// TODO_gui
	//if (auto* e = std::get_if<sf::Event>(&phEvent))
	//{
	//	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	//	{
	//		auto c = sf::Mouse::getPosition(mGameData->getWindow());
	//		auto distanceFromCenter = mWindow->mapPixelToCoords(c);
	//		if (mIsIconChecked || isMouseOnSliderIcon(distanceFromCenter))
	//		{
	//			mIsIconChecked = true;
	//			mIcon->setPosition({ getNewIconXPosition(distanceFromCenter.x, mSize.x, mIcon->getSize().x), 0});
	//		}
	//		for (const auto& k : mBehaviors)
	//			if (k.first == BehaviorType::onPressed)
	//				k.second(this);
	//	}
	//	else mIsIconChecked = false;
	//}
}

bool SliderWidget::isMouseOnSliderIcon(const sf::Vector2f& distanceFromCenter)
{
	// TODO_gui
	return false;
	/*return mIcon->getGlobalPosition().x < distanceFromCenter.x
		&& mIcon->getGlobalPosition().x + mIcon->getSize().x > distanceFromCenter.x
		&& mIcon->getGlobalPosition().y < distanceFromCenter.y
		&& mIcon->getGlobalPosition().y + mIcon->getSize().y > distanceFromCenter.y;*/
}

}

