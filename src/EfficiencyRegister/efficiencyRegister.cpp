#include "efficiencyRegister.hpp"
#include "gameData.hpp"

namespace ph {

EfficiencyRegister::EfficiencyRegister()
	:mEfficiencyDisplayer()
	,mClock()
	,mGameData(nullptr)
	,mFramesPerSecond(0)
	,mFramesFromLastSecond(0)
	,mIsFPSCounterActive(false)
	,mIsRendererDebugActive(false)
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
			mIsFPSCounterActive = !mIsFPSCounterActive;
			mEfficiencyDisplayer.setShouldFPSBeDrawn(mIsFPSCounterActive);
		}
		else if(e->type == sf::Event::KeyPressed && e->key.code == sf::Keyboard::F3) {
			mIsRendererDebugActive = !mIsRendererDebugActive;
			mEfficiencyDisplayer.setShouldRendererDebugBeDrawn(mIsRendererDebugActive);
		}
	}
}

void EfficiencyRegister::update()
{
	if(mIsFPSCounterActive)
		mEfficiencyDisplayer.setFramePerSecondText("FPS:  " + std::to_string(mFramesPerSecond));

	if(mClock.getElapsedTime().asSeconds() >= 1) {
		mFramesPerSecond = mFramesFromLastSecond;
		mClock.restart();
		mFramesFromLastSecond = 0;
	}
	else
		++mFramesFromLastSecond;
}

void EfficiencyRegister::setAllDrawCallsPerFrame(unsigned alldrawCallsPerFrame)
{
	if(mIsRendererDebugActive)
		mEfficiencyDisplayer.setDrawCallPerFrameText("All Draw calls per frame: " + std::to_string(alldrawCallsPerFrame));
}

void EfficiencyRegister::setNumberOfSFMLDrawCalls(unsigned nrOfDrawnSprites)
{
	if(mIsRendererDebugActive)
		mEfficiencyDisplayer.setSFMLDrawCalls("SFML draw calls: " + std::to_string(nrOfDrawnSprites));
}

void EfficiencyRegister::setNumberOfInstancedDrawCalls(unsigned nrOfInstancedDrawCalls)
{
	if(mIsRendererDebugActive)
		mEfficiencyDisplayer.setInstancedDrawCalls("Instanced draw calls: " + std::to_string(nrOfInstancedDrawCalls));
}

void EfficiencyRegister::setNumberOfDrawnInstancedSprites(unsigned nrOfDrawnInstancedSprites)
{
	if(mIsRendererDebugActive)
		mEfficiencyDisplayer.setDrawnInstancedSprites("Drawn instanced sprites: " + std::to_string(nrOfDrawnInstancedSprites));
}

void EfficiencyRegister::setNumberOfTexturesDrawnByInstancedRendering(unsigned nrOfTexturesDrawnByInstancedRendering)
{
	if(mIsRendererDebugActive)
		mEfficiencyDisplayer.setTexturesDrawnByInstancing("Drawn textures by instancing: " + std::to_string(nrOfTexturesDrawnByInstancedRendering));
}

void EfficiencyRegister::setNumberOfLineDrawCalls(unsigned nrOfLineDrawCalls)
{
	if(mIsRendererDebugActive)
		mEfficiencyDisplayer.setLineDrawCalls("Line draw calls: " + std::to_string(nrOfLineDrawCalls));
}

void EfficiencyRegister::setNumberOfDrawnLines(unsigned nrOfDrawnLines)
{
	if(mIsRendererDebugActive)
		mEfficiencyDisplayer.setDrawnLines("Drawn lines: " + std::to_string(nrOfDrawnLines));
}

void EfficiencyRegister::setNumberOfDrawnPoints(unsigned nrOfDrawnPoints)
{
	if(mIsRendererDebugActive)
		mEfficiencyDisplayer.setDrawnPoints("Drawn points: " + std::to_string(nrOfDrawnPoints));
}

void EfficiencyRegister::setNumberOfPointDrawCalls(unsigned nrOfDrawCalls)
{
	if(mIsRendererDebugActive)
		mEfficiencyDisplayer.setPointDrawCalls("Point draw calls: " + std::to_string(nrOfDrawCalls));
}

}
