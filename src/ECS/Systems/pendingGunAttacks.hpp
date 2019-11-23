#pragma once

#include "ECS/system.hpp"
#include "Utilities/rect.hpp"

#include <SFML/System/Vector2.hpp>

namespace ph::system {

	class PendingGunAttacks : public System
	{
	public:
		using System::System;

		void update(float seconds) override;

	private:
		sf::Vector2f performShoot(const sf::Vector2f& startingBulletPos);
		void createShotImage(const sf::Vector2f& startingPosition, const sf::Vector2f& endingPosition);
		void setPlayerFacePosition();
		bool canShoot(int numOfBullets, float cooldown) const;
		bool hasCooldown(float cooldownSinceLastShoot) const;
		sf::Vector2f getGunPosition() const;
		sf::Vector2f getCurrentPosition(const sf::Vector2f& startingPos, const int bulletDistance) const;


	private:
		sf::Vector2f mPlayerFaceDirection;
	};
}
