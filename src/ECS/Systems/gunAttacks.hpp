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
		void onEvent(Event) override;

	private:
		void handlePendingGunAttacks() const;
		sf::Vector2f getBulletStartingPosition(const sf::Vector2f& playerFaceDirection) const;
		void tagEnemiesInGunAttackArea(sf::Vector2f playerFaceDirection, const FloatRect& playerBody, sf::Vector2f gunSize, float range, float deflectionAngle) const;
		std::vector<sf::Vector2f> performShoot(const sf::Vector2f& playerFaceDirection, const sf::Vector2f& startingBulletPos, float range, float deflectionAngle, int damage, int numberOfBullets) const;
		sf::Vector2f getBulletDirection(const sf::Vector2f& playerFaceDirection, float deflection) const;
		sf::Vector2f getCurrentPosition(const sf::Vector2f& bulletDirection, const sf::Vector2f& startingPos, const int bulletDistance) const;
		void clearInGunAttackAreaTags() const;

		void createShotImage(const sf::Vector2f shotsStartingPosition, const std::vector<sf::Vector2f>& shots, const std::string& soundFilename) const;
		void handleLastingBullets() const;
	};
}
