#pragma once

#include <SFML/Graphics.hpp>

namespace ph {

class GameData;

class EfficencyRegister : public sf::Drawable
{
public:
	EfficencyRegister();

	void update() const;
	void draw(sf::RenderTarget&, const sf::RenderStates) const override;

	void registerRenderCall() { ++mRenderCallPerFrame; }

private:
	sf::Text mFramesPerSecondText;
	sf::Text mRenderCallPerFrameText;
	int mFramesPerSecond;
	int mRenderCallPerFrame;
};

}
