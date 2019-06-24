#pragma once

#include <SFML/Graphics.hpp>

namespace ph {

class GameData;

class EfficencyRegister : public sf::Drawable
{
public:
	EfficencyRegister();
	void init(GameData* const gameData);

	void input();
	void update();
	void draw(sf::RenderTarget&, const sf::RenderStates) const override;

	void move(sf::Vector2f offset);
	void registerRenderCall() { ++mRenderCallPerFrame; }

private:
	sf::Text mFramesPerSecondText;
	sf::Text mRenderCallPerFrameText;
	sf::RectangleShape mBackground;
	sf::Clock mClock;
	GameData* mGameData;
	unsigned mFramesPerSecond;
	unsigned mRenderCallPerFrame;
	unsigned mFramesFromLastSecond;
	bool mShouldBeDrawn;
};

}
