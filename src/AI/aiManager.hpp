#pragma once 

#include "pathData.hpp"
#include <SFML/Graphics.hpp>
#include <deque>

namespace ph { 

class AIManager
{
public:
	Path getZombiePath(const sf::Vector2f zombiePosition) const;
	bool shouldZombiePlayAttackAnimation(const sf::Vector2f zombiePosition) const;

	void setIsPlayerOnScene(bool isPlayerOnScene) { mIsPlayerOnScene = isPlayerOnScene; }
	void setPlayerPosition(const sf::Vector2f playerPosition);
	bool hasPlayerMovedSinceLastUpdate() const { return mHasPlayerMovedSinceLastUpdate; }
	void registerMapSize(const sf::Vector2u mapSizeInTiles);
	void registerObstacle(const sf::Vector2f collisionBodyPosition);

	void update();

private:
	bool doesZombieSeePlayer(const sf::Vector2f zombiePosition) const;
	Path getPath(const sf::Vector2f startPosition, const sf::Vector2f destinationPosition) const;
	sf::Vector2u toNodePosition(sf::Vector2f) const;
	Path getRandomPath(const sf::Vector2f startPosition) const;

private:
	ObstacleGrids mObstacleGrid;
	sf::Vector2f mPlayerPosition;
	const unsigned mSpotSideLength = 16;
	bool mHasPlayerMovedSinceLastUpdate = false;
	bool mIsPlayerOnScene = false;
};

} 
