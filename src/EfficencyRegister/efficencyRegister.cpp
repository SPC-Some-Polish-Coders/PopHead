#include "efficencyRegister.hpp"
#include "gameData.hpp"

namespace ph {

EfficencyRegister::EfficencyRegister()
	:mEfficencyDisplayer()
	,mClock()
	,mFramesPerSecond(0)
	,mRenderCallPerFrame(0)
	,mFramesFromLastSecond(0)
	,mIsActive(false)
{
}

void EfficencyRegister::init(GameData* const gameData)
{
	mGameData = gameData;
	mEfficencyDisplayer.init(gameData);
}

void EfficencyRegister::input()
{
	if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::F2)) {
		mIsActive = !mIsActive;
		mEfficencyDisplayer.setShouldBeDrawn(mIsActive);
	}
}

void EfficencyRegister::update()
{
	if(mIsActive) {
		mEfficencyDisplayer.setFramePerSecondText("FPS:  " + std::to_string(mFramesPerSecond));
		mEfficencyDisplayer.setRenderCallPerFrameText("DCPF: " + std::to_string(mRenderCallPerFrame));
		mRenderCallPerFrame = 0;
	}

	if(mClock.getElapsedTime().asSeconds() >= 1) {
		mFramesPerSecond = mFramesFromLastSecond;
		mClock.restart();
		mFramesFromLastSecond = 0;
	}
	else
		++mFramesFromLastSecond;
}

}