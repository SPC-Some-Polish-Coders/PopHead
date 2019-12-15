#pragma once

#include "ECS/system.hpp"
#include "Utilities/rect.hpp"

#include <SFML/System.hpp>

namespace ph::system {

	class MeleePositioning : public System
	{
	public:
		using System::System;

		void update(float seconds) override;

	private:
		sf::Vector2f getRightHandPosition(const sf::Vector2f& playerFaceDirection);
		void updateMeleeSpritePosition(const sf::Vector2f& playerFaceDirection, const sf::Vector2f& playerPosition, FloatRect& meleeBody);
		void updateTexture(bool isAttacking, float dt, const sf::Vector2f playerPosition);

	};
}
