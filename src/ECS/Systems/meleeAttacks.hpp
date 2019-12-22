#pragma once

#include "ECS/system.hpp"
#include "Utilities/rect.hpp"

#include <SFML/System/Vector2.hpp>

namespace ph::system {

	class MeleeAttacks : public System
	{
	public:
		using System::System;

		void update(float dt) override;

	private:
		void tagEnemiesInMeleeAttackArea(sf::Vector2f playerFaceDirection, const FloatRect& playerBody, float range) const;
		void performHit(const sf::Vector2f playerCenterPosition, float weaponInitialRotation, int damage, float range, float rotationRange);

		sf::Vector2f nearestPointOfCharacter(const FloatRect& rect, const sf::Vector2f playerPosition) const;
		float angleOfPointToStart(sf::Vector2f point, const sf::Vector2f& playerPosition) const;
		bool isAngleInAttackRange(float angle, float mAttackAngle, float rotationRange) const;
		float getFixedAngle(float angle) const;

		float getStartAttackRotation(const sf::Vector2f& playerFaceDirection) const;
		void clearInMeleeAttackAreaTags() const;

	};
}
