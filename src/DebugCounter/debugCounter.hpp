#pragma once

#include "Events/event.hpp"
#include "Resources/resourceHolder.hpp"
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Clock.hpp>

namespace ph {

class DebugCounter
{
public:
	DebugCounter();
	void init(FontHolder& fonts);

	void handleEvent(const ph::Event&);
	void update();
	void draw() const;

	void setAllDrawCallsPerFrame(unsigned alldrawCallsPerFrame);
	void setNumberOfSFMLDrawCalls(unsigned nrOfDrawnSprites);
	void setNumberOfInstancedDrawCalls(unsigned nrOfInstancedDrawCalls);
	void setNumberOfRenderGroups(unsigned nrOfRenderGroups);
	void setNumberOfDrawnInstancedSprites(unsigned nrOfDrawnInstancedSprites);
	void setNumberOfTexturesDrawnByInstancedRendering(unsigned nrOfTexturesDrawnByInstancedRendering);
	void setNumberOfLineDrawCalls(unsigned nrOfTexturesDrawnByInstancedRendering);
	void setNumberOfDrawnLines(unsigned nrOfTexturesDrawnByInstancedRendering);
	void setNumberOfDrawnPoints(unsigned nrOfDrawnPoints);
	void setNumberOfPointDrawCalls(unsigned nrOfDrawCalls);

private:
	sf::Text mFPSText;
	sf::Text mAllDrawCallsText;
	sf::Text mSFMLDrawCallsText;
	sf::Text mInstancedDrawCallsText;
	sf::Text mRenderGroupsInQuadRendererText;
	sf::Text mDrawnInstancedSpritesText;
	sf::Text mTexturesDrawnByInstancingText;
	sf::Text mLineDrawCallsText;
	sf::Text mDrawnLinesText;
	sf::Text mPointDrawCallsText;
	sf::Text mDrawnPointsText;
	sf::RectangleShape mFPSBackground;
	sf::RectangleShape mRendererDebugBackground;
	sf::Clock mClock;
	unsigned mFPS;
	unsigned mFramesFromLastSecond;
	bool mIsFPSCounterActive;
	bool mIsRendererDebugActive;
};

}
