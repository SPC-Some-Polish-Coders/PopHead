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

	void move(sf::Vector2f offset);

	void setShouldFPSBeDrawn(const bool shouldFPSBeDrawn);
	void setShouldRendererDebugBeDrawn(const bool shouldRendererDebugBeDrawn);

	void setFramePerSecondText(const std::string& text);
	void setDrawCallPerFrameText(const std::string& text);
	void setNumberOfDrawnSpritesText(const std::string& text);

private:
	sf::Text mFramesPerSecondText;
	sf::Text mDrawCallPerFrameText;
	sf::Text mNumberOfDrawnSprites;
	sf::RectangleShape mFPSBackground;
	sf::RectangleShape mRendererDebugBackground;
	bool mShouldFPSBeDrawn;
	bool mShouldRendererDebugBeDrawn;
};

}