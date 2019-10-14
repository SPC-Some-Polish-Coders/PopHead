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

	void setShouldBeDrawn(const bool shouldBeDrawn);
	void setFramePerSecondText(const std::string& text);
	void setDrawCallPerFrameText(const std::string& text);

private:
	sf::Text mFramesPerSecondText;
	sf::Text mDrawCallPerFrameText;
	sf::RectangleShape mBackground;
	bool mShouldBeDrawn;
};

}