#pragma once

#include "ECS/system.hpp"

#include <SFML/System/Vector2.hpp>

namespace ph::system {

	class PlayerMovementInput : public System
	{
	public:
		using System::System;

		void update(float seconds) override;

	private:
		sf::Vector2f getPlayerDirection() const;
		void updateInputFrags();
		void updateAnimationData();
		void setPlayerFaceDirection(const sf::Vector2f& faceDirection) const;

	private:
		bool mUp;
		bool mDown;
		bool mLeft;
		bool mRight;
	};

	class PlayerAttackType : public System
	{
	public:
		using System::System;

		void update(float seconds) override;
	};
}
