#include "efficiencyRegister.hpp"
#include "gameData.hpp"

namespace ph {

EfficiencyRegister::EfficiencyRegister()
	:mEfficiencyDisplayer()
	,mClock()
	,mGameData(nullptr)
	,mFramesPerSecond(0)
	,mFramesFromLastSecond(0)
	,mIsActive(false)
{
}

void EfficiencyRegister::init(GameData* const gameData)
{
	mGameData = gameData;
	mEfficiencyDisplayer.init(gameData);
}

void EfficiencyRegister::handleEvent(const ph::Event& phEvent)
{
	if(auto* e = std::get_if<sf::Event>(&phEvent))
	{
		if(e->type == sf::Event::KeyPressed && e->key.code == sf::Keyboard::F2) {
			mIsActive = !mIsActive;
			mEfficiencyDisplayer.setShouldBeDrawn(mIsActive);
		}
	}
}

void EfficiencyRegister::update()
{
	if(mIsActive)
		mEfficiencyDisplayer.setFramePerSecondText("FPS:  " + std::to_string(mFramesPerSecond));

	if(mClock.getElapsedTime().asSeconds() >= 1) {
		mFramesPerSecond = mFramesFromLastSecond;
		mClock.restart();
		mFramesFromLastSecond = 0;
	}
	else
		++mFramesFromLastSecond;
}

void EfficiencyRegister::setDrawCallsPerFrame(unsigned drawCallsPerFrame)
{
	if(mIsActive)
		mEfficiencyDisplayer.setDrawCallPerFrameText("DCPF: " + std::to_string(drawCallsPerFrame));
}

}
