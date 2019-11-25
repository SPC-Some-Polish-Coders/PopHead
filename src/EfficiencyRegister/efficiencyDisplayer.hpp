#pragma once

#include <SFML/Graphics.hpp>

namespace ph {

class GameData;

class EfficiencyDisplayer
{
public:
	EfficiencyDisplayer();
	void init(GameData* const gameData);

	void draw() const;

	void setShouldFPSBeDrawn(const bool shouldFPSBeDrawn);
	void setShouldRendererDebugBeDrawn(const bool shouldRendererDebugBeDrawn);

	void setFramePerSecondText(const std::string& text);
	void setDrawCallPerFrameText(const std::string& text);
	void setSFMLDrawCalls(const std::string& text);
	void setInstancedDrawCalls(const std::string& text);
	void setDrawnInstancedSprites(const std::string& text);
	void setTexturesDrawnByInstancing(const std::string& text);
	void setLineDrawCalls(const std::string& text);
	void setDrawnLines(const std::string& text);
	void setPointDrawCalls(const std::string& text);
	void setDrawnPoints(const std::string& text);

private:
	sf::Text mFramesPerSecondText;
	sf::Text mDrawCallPerFrameText;
	sf::Text mSFMLDrawCalls;
	sf::Text mInstancedDrawCalls;
	sf::Text mDrawnInstancedSprites;
	sf::Text mTexturesDrawnByInstancing;
	sf::Text mLineDrawCalls;
	sf::Text mDrawnLines;
	sf::Text mPointDrawCalls;
	sf::Text mDrawnPoints;
	sf::RectangleShape mFPSBackground;
	sf::RectangleShape mRendererDebugBackground;
	bool mShouldFPSBeDrawn;
	bool mShouldRendererDebugBeDrawn;
};

}