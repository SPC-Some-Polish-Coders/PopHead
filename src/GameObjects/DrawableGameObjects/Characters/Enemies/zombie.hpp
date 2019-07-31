#pragma once

#include "GameObjects/DrawableGameObjects/Characters/enemy.hpp"
#include "AI/aiManager.hpp"

namespace ph {

class Zombie : public Enemy
{
public:
	Zombie(GameData*);

	void update(sf::Time delta) override;
private:
	void move(sf::Time delta);
	sf::Vector2f toDirectionVector(Direction);

private:
	sf::Clock timeFromLastGrowl;

	Path mMovementPath;
	sf::Vector2f mCurrentDirectionVector;
	sf::Clock mTimeFromStartingThisMove;
	static constexpr float mTimeInSecondsToMoveToAnotherTile = 0.2f;
};

}
