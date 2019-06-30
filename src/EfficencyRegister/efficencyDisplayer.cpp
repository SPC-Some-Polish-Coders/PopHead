#include "efficencyDisplayer.hpp"
#include "gameData.hpp"

namespace ph {

EfficencyDisplayer::EfficencyDisplayer()
	:mShouldBeDrawn(false)
{
}

void EfficencyDisplayer::init(GameData* const gameData)
{
	mBackground.setFillColor(sf::Color(0, 0, 0, 230));
	mBackground.setPosition(220, -220);
	mBackground.setSize({100, 25});

	const sf::Font& font(gameData->getFonts().get("fonts/joystixMonospace.ttf"));

	mFramesPerSecondText.setFont(font);
	mFramesPerSecondText.setPosition(220, -220);
	mFramesPerSecondText.setCharacterSize(10);

	mRenderCallPerFrameText.setFont(font);
	mRenderCallPerFrameText.setPosition(220, -208);
	mRenderCallPerFrameText.setCharacterSize(10);
}

void EfficencyDisplayer::draw(sf::RenderTarget& target, const sf::RenderStates states) const
{
	if(mShouldBeDrawn) {
		target.draw(mBackground, states);
		target.draw(mFramesPerSecondText, states);
		target.draw(mRenderCallPerFrameText, states);
	}
}

void EfficencyDisplayer::move(sf::Vector2f offset)
{
	mBackground.move(offset);
	mFramesPerSecondText.move(offset);
	mRenderCallPerFrameText.move(offset);
}

}