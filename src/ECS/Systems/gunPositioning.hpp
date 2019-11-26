#pragma once

#include "ECS/system.hpp"
#include "Utilities/rect.hpp"

#include <SFML/System/Vector2.hpp>

namespace ph::system {

class GunPositioning : public System
{
public:
	using System::System;

	void update(float seconds) override;

private:
	void updateGunSpriteFlipping(const sf::Vector2f& playerFaceDirection, FloatRect& gunBody);
	void updateGunSpritePosition(const sf::Vector2f& playerFaceDirection, const sf::Vector2f& playerPosition, FloatRect& gunBody);
	sf::Vector2f getRightHandPosition(const sf::Vector2f& playerFaceDirection);
};

}
