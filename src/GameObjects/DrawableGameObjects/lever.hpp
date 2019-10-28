#pragma once

#include "GameObjects/gameObject.hpp"
#include "Renderer/renderer.hpp"

namespace ph {

class Lever : public GameObject
{
public:
	Lever(const Texture& leverTexture, const Texture& hintTexture);

	void updateCurrent(const sf::Time delta) override;
	void drawCurrent(sf::Transform) override;

private:
	IntRect mCurrentTextureRect;
	const Texture& mLeverTexture;
	const Texture& mHintTexture;
	bool mIsLeverDown;
	bool mIsPlayerInHintArea;
};

}
