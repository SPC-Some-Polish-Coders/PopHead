
#pragma once

#include "GameObjects/DrawableGameObjects/Characters/enemy.hpp"
#include "AI/aiManager.hpp"

namespace ph {

	class SlowZombie : public Enemy
	{
	public:
		SlowZombie(GameData*);

		void updateCurrent(sf::Time delta) override;

		void slowDown() { mIsSlownDown = true; }

	private:
		void handlePlayerHit();
		void move(sf::Time delta);
		sf::Vector2f toDirectionVector(Direction);
		void updateAnimation(sf::Time delta);
		void setAnimationState(const std::string& stateName);

	private:
		PathMode mMovementPath;
		sf::Clock timeFromLastGrowl;
		sf::Clock mTimeFromStartingThisMove;
		sf::Vector2f mCurrentDirectionVector;
		bool mIsSlownDown;
		static constexpr float mTimeInSecondsToMoveToAnotherTile = 0.15f;
	};

}
