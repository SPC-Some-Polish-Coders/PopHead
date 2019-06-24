#include "efficencyRegister.hpp"
#include "gameData.hpp"

namespace ph {

EfficencyRegister::EfficencyRegister()
	:mBackground({60, 25})
	,mFramesPerSecond(0)
	,mRenderCallPerFrame(0)
	,mShouldBeDrawn(false)
{
}

void EfficencyRegister::init(GameData* const gameData)
{
	mGameData = gameData;
	mBackground.setFillColor(sf::Color(0, 0, 0, 230));
	mBackground.setPosition(220, -220);
	const sf::Font& font(mGameData->getFonts().get("fonts/joystixMonospace.ttf"));
	mFramesPerSecondText.setFont(font);
	mFramesPerSecondText.setPosition(220, -220);
	mFramesPerSecondText.setCharacterSize(10);
	mRenderCallPerFrameText.setFont(font);
	mRenderCallPerFrameText.setPosition(220, -208);
	mRenderCallPerFrameText.setCharacterSize(10);
}

void EfficencyRegister::input()
{
	if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::F2))
		mShouldBeDrawn = !mShouldBeDrawn;
}

void EfficencyRegister::update()
{
	if(mShouldBeDrawn) {
		mFramesPerSecondText.setString("FPS:  " + std::to_string(mFramesPerSecond));
		mRenderCallPerFrameText.setString("RCPS: " + std::to_string(mRenderCallPerFrame));
	}
}

void EfficencyRegister::draw(sf::RenderTarget& target, const sf::RenderStates states) const
{
	if(mShouldBeDrawn) {
		target.draw(mBackground, states);
		target.draw(mFramesPerSecondText, states);
		target.draw(mRenderCallPerFrameText, states);
	}
}

void EfficencyRegister::move(sf::Vector2f offset)
{
	mBackground.move(offset);
	mFramesPerSecondText.move(offset);
	mRenderCallPerFrameText.move(offset);
}

}