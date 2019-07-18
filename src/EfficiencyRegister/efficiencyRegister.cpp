#include "efficiencyRegister.hpp"
#include "gameData.hpp"

namespace ph {

EfficiencyRegister::EfficiencyRegister()
	:mEfficiencyDisplayer()
	,mClock()
	,mFramesPerSecond(0)
	,mRenderCallPerFrame(0)
	,mFramesFromLastSecond(0)
	,mIsActive(false)
{
}

void EfficiencyRegister::init(GameData* const gameData)
{
	mGameData = gameData;
	mEfficiencyDisplayer.init(gameData);
}

void EfficiencyRegister::input()
{
	if(mGameData->getInput().getKeyboard().isKeyJustPressed(sf::Keyboard::F2)) {
		mIsActive = !mIsActive;
		mEfficiencyDisplayer.setShouldBeDrawn(mIsActive);
	}
}

void EfficiencyRegister::update()
{
	if(mIsActive) {
		mEfficiencyDisplayer.setFramePerSecondText("FPS:  " + std::to_string(mFramesPerSecond));
		mEfficiencyDisplayer.setRenderCallPerFrameText("DCPF: " + std::to_string(mRenderCallPerFrame));
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