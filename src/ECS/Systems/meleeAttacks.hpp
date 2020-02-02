#pragma once

#include "ECS/system.hpp"
#include "Utilities/rect.hpp"

#include <SFML/System/Vector2.hpp>

namespace ph::system {

	class MeleeAttacks : public System
	{
	public:
		using System::System;

		void onEvent(Event)override;
		void update(float dt) override;

	private:
		float getStartAttackRotation(const sf::Vector2f& playerFaceDirection) const;

	private:
		float mStartWeaponRotation;
		bool mIsAttackButtonPressed = false;
		bool mShouldWeaponBeRendered = false;
	};
}

