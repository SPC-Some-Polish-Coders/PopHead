#pragma once

#include "ECS/system.hpp"
#include "Utilities/rect.hpp"

#include <SFML/System/Vector2.hpp>

namespace ph::system {

	class PendingMeleeAttacks : public System
	{
	public:
		using System::System;

		void update(float seconds) override;

	private:
		void performHit(const sf::Vector2f playerCenterPosition, float weaponInitialRotation);

		sf::Vector2f nearestPointOfCharacter(const FloatRect& rect, const sf::Vector2f playerPosition) const;
		float angleOfPointToStart(sf::Vector2f point, const sf::Vector2f& playerPosition) const;
		bool isAngleInAttackRange(float angle, float mAttackAngle) const;
		float getFixedAngle(float angle) const;

		float getStartAttackRotation(const sf::Vector2f& playerFaceDirection) const;

	};
}
