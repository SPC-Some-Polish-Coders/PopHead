#pragma once

#include "GameObjects/gameObject.hpp"
#include "Renderer/renderer.hpp"
#include <string>

namespace ph {

class DeadCharacter : public GameObject
{
public:
	DeadCharacter(const Texture& texture, const sf::IntRect& textureRect, const std::string name);

	void updateCurrent(const sf::Time delta) override;
	void drawCurrent(sf::Transform) override;

private:
	const sf::IntRect mTextureRect;
	sf::Time mTimeFromDeath;
	const Texture& mTexture;
};

}