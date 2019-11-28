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
	void initFPSCounter();
	void initRendererDebug();

private:
	struct FPSCounter
	{
		sf::Text fpsText;
		sf::RectangleShape fpsBackground;
	};	
	FPSCounter* mFPSCounter;

	struct RendererDebug
	{
		sf::Text allDrawCallsText;
		sf::Text sfmlDrawCallsText;
		sf::Text instancedDrawCallsText;
		sf::Text renderGroupsInQuadRendererText;
		sf::Text drawnInstancedSpritesText;
		sf::Text texturesDrawnByInstancingText;
		sf::Text lineDrawCallsText;
		sf::Text drawnLinesText;
		sf::Text pointDrawCallsText;
		sf::Text drawnPointsText;
		sf::RectangleShape rendererDebugBackground;
	};
	RendererDebug* mRendererDebug;	

	sf::Font* mFont;

	sf::Clock mClock;
	unsigned mFPS;
	unsigned mFramesFromLastSecond;
	bool mIsFPSCounterActive;
	bool mIsRendererDebugActive;
};

}
