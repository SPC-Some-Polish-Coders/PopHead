#include "efficiencyDisplayer.hpp"
#include "gameData.hpp"

namespace ph {

EfficiencyDisplayer::EfficiencyDisplayer()
	:mShouldBeDrawn(false)
{
}

void EfficiencyDisplayer::init(GameData* const gameData)
{
	mBackground.setFillColor(sf::Color(0, 0, 0, 230));
	mBackground.setPosition(220, -220);
	mBackground.setSize({100, 25});

	const sf::Font& font(gameData->getFonts().get("fonts/joystixMonospace.ttf"));

	mFramesPerSecondText.setFont(font);
	mFramesPerSecondText.setPosition(220, -220);
	mFramesPerSecondText.setCharacterSize(10);

	mDrawCallPerFrameText.setFont(font);
	mDrawCallPerFrameText.setPosition(220, -208);
	mDrawCallPerFrameText.setCharacterSize(10);
}

void EfficiencyDisplayer::draw(sf::RenderTarget& target, const sf::RenderStates states) const
{
	if(mShouldBeDrawn) {
		target.draw(mBackground, states);
		target.draw(mFramesPerSecondText, states);
		target.draw(mDrawCallPerFrameText, states);
	}
}

void EfficiencyDisplayer::move(sf::Vector2f offset)
{
	mBackground.move(offset);
	mFramesPerSecondText.move(offset);
	mDrawCallPerFrameText.move(offset);
}

void EfficiencyDisplayer::setShouldBeDrawn(const bool shouldBeDrawn)
{
	mShouldBeDrawn = shouldBeDrawn;
}

void EfficiencyDisplayer::setFramePerSecondText(const std::string& text)
{
	mFramesPerSecondText.setString(text);
}

void EfficiencyDisplayer::setDrawCallPerFrameText(const std::string& text)
{
	mDrawCallPerFrameText.setString(text);
}

}
