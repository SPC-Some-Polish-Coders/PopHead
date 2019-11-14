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
		void setPlayerFacePosition();
		void performHit(const sf::Vector2f& startingWeaponPosition);
		bool hasCooldown(float cooldownSinceLastHit) const;

	private:
		sf::Vector2f mPlayerFaceDirection;
	};
}
