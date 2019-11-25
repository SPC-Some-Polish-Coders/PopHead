#pragma once

#include "efficiencyDisplayer.hpp"
#include "Events/event.hpp"
#include <SFML/Graphics.hpp>

namespace ph {

class GameData;

class EfficiencyRegister
{
public:
	EfficiencyRegister();
	void init(GameData* const gameData);

	void handleEvent(const ph::Event&);
	void update();

	void setAllDrawCallsPerFrame(unsigned alldrawCallsPerFrame);
	void setNumberOfSFMLDrawCalls(unsigned nrOfDrawnSprites);
	void setNumberOfInstancedDrawCalls(unsigned nrOfInstancedDrawCalls);
	void setNumberOfDrawnInstancedSprites(unsigned nrOfDrawnInstancedSprites);
	void setNumberOfTexturesDrawnByInstancedRendering(unsigned nrOfTexturesDrawnByInstancedRendering);
	void setNumberOfLineDrawCalls(unsigned nrOfTexturesDrawnByInstancedRendering);
	void setNumberOfDrawnLines(unsigned nrOfTexturesDrawnByInstancedRendering);
	void setNumberOfDrawnPoints(unsigned nrOfDrawnPoints);
	void setNumberOfPointDrawCalls(unsigned nrOfDrawCalls);

	auto getDisplayer() -> EfficiencyDisplayer& { return mEfficiencyDisplayer; }

private:
	EfficiencyDisplayer mEfficiencyDisplayer;
	sf::Clock mClock;
	GameData* mGameData;
	unsigned mFramesPerSecond;
	unsigned mFramesFromLastSecond;
	bool mIsFPSCounterActive;
	bool mIsRendererDebugActive;
};

}
