#pragma once 

#include "pathData.hpp"
#include <SFML/Graphics.hpp>
#include <deque>

namespace ph { 

class AIManager
{
public:
	Path getZombiePath(const sf::Vector2f zombiePosition) const;

	void setPlayerPosition(const sf::Vector2f playerPosition) { this->mPlayerPosition = playerPosition; }
	void registerMapSize(const sf::Vector2u mapSizeInTiles);
	void registerObstacle(const sf::Vector2f collisionBodyPosition);

private:
	bool doesZombieSeePlayer(const sf::Vector2f zombiePosition) const;
	Path getPath(const sf::Vector2f startPosition, const sf::Vector2f destination) const;
	Path getRandomPath(const sf::Vector2f startPosition) const;

private:
	ObstacleGrid mGrid;
	sf::Vector2f mPlayerPosition;
	const unsigned mSpotSideLength = 16;
};

} 
