#pragma once

#include "ECS/system.hpp"

#include <SFML/System/Vector2.hpp>

namespace ph::system {

	class PlayerMovementInput : public System
	{
	public:
		using System::System;

		void update(float dt) override;

	private:
		bool isPlayerWithoutControl();
		sf::Vector2f getPlayerDirection() const;
		void updateInputFlags();
		void updateAnimationData();
		void setPlayerFaceDirection(const sf::Vector2f faceDirection) const;
		void setFlashLightDirection(const sf::Vector2f faceDirection) const;
		void updateGunAttackInput();
		void updateMeleeAttackInput();

	private:
		bool mUp;
		bool mDown;
		bool mLeft;
		bool mRight;
	};
}
