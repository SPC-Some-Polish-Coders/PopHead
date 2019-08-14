#pragma once

#include "GameObjects/DrawableGameObjects/Characters/enemy.hpp"
#include "AI/aiManager.hpp"

namespace ph {

class Zombie : public Enemy
{
public:
	Zombie(GameData*);

	void updateCurrent(sf::Time delta) override;
private:
	void deathUpdate();
	void handlePlayerHit();
	void move(sf::Time delta);
	sf::Vector2f toDirectionVector(Direction);
	void updateAnimation(sf::Time delta);
	void setAnimationState(const std::string& stateName);

private:
	Path mMovementPath;
	sf::Clock timeFromLastGrowl;
	sf::Clock mTimeFromStartingThisMove;
	sf::Clock mTimeFromDeath;
	sf::Vector2f mCurrentDirectionVector;
	static constexpr float mTimeInSecondsToMoveToAnotherTile = 0.2f;
};

}
