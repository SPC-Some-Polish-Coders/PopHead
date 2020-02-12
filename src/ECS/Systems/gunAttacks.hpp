#pragma once

#include "ECS/system.hpp"
#include "Utilities/rect.hpp"
#include <SFML/System/Vector2.hpp>

namespace ph::system {

	class GunAttacks : public System
	{
	public:
		using System::System;

		void update(float dt) override;
		void onEvent(sf::Event) override;

	private:
		void handlePendingGunAttacks() const;
		sf::Vector2f getBulletStartingPosition(sf::Vector2f playerFaceDirection) const;
		void tagEnemiesInGunAttackArea(sf::Vector2f playerFaceDirection, FloatRect playerBody, sf::Vector2f gunSize,
		                               float range, float deflectionAngle, bool* wasOpponentHitOut) const;
		std::vector<sf::Vector2f> performShoot(sf::Vector2f playerFaceDirection, sf::Vector2f startingBulletPos, float range,
		                                       float deflectionAngle, int damage, int numberOfBullets) const;
		sf::Vector2f getBulletDirection(sf::Vector2f playerFaceDirection, float deflection) const;
		sf::Vector2f getCurrentPosition(sf::Vector2f bulletDirection, sf::Vector2f startingPos, int bulletDistance) const;
		void clearInGunAttackAreaTags() const;

		void createShotImage(sf::Vector2f shotsStartingPosition, const std::vector<sf::Vector2f>& shots, const std::string& soundFilename) const;
		void handleLastingBullets() const;
		void shakeCamera(float magnitude, bool smooth) const; 
	};
}
