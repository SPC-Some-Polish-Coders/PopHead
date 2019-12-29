#pragma once

#include "ECS/system.hpp"
#include "Utilities/rect.hpp"

#include <SFML/System/Vector2.hpp>

namespace ph::system {

class GunPositioningAndTexture : public System
{
public:
	using System::System;

	void update(float dt) override;

private:
	void updateTexture(float dt, sf::Vector2f playerFaceDirection, bool wantToAttack) const;

	void updateGunPosition(sf::Vector2f playerFaceDirection, const FloatRect& playerBody) const;
	sf::Vector2f getGunSpriteFlipping(sf::Vector2f playerFaceDirection, sf::Vector2f gunSize) const;
	sf::Vector2f getGunNewSpritePosition(sf::Vector2f playerFaceDirection, const FloatRect& playerBody, const FloatRect& gunBody) const;
	sf::Vector2f getGunOffset(sf::Vector2f playerFaceDirection, sf::Vector2f playerBodySize) const;


};

}
