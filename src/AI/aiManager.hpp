#pragma once 

#include "pathData.hpp"
#include "obstacleGrid.hpp"
#include <SFML/Graphics.hpp>
#include <deque>

namespace ph { 

enum class AIMode {
	normal,
	zombieAlwaysWalkRandomly
};

class AIManager
{
public:
	Path getZombiePath(const sf::Vector2f zombiePosition) const;
	bool shouldZombiePlayAttackAnimation(const sf::Vector2f zombiePosition) const;

	void setAIMode(const AIMode aiMode) { mAIMode = aiMode; }
	void setIsPlayerOnScene(bool isPlayerOnScene) { mIsPlayerOnScene = isPlayerOnScene; }
	bool isPlayerOnScene() const { return mIsPlayerOnScene; }
	void setPlayerPosition(const sf::Vector2f playerPosition);
	bool hasPlayerMovedSinceLastUpdate() const { return mHasPlayerMovedSinceLastUpdate; }
	void registerMapSize(const sf::Vector2u mapSizeInTiles);
	void registerObstacle(const sf::Vector2f collisionBodyPosition);

	void update();

private:
	float getDistanceBetweenZombieAndPlayer(const sf::Vector2f zombiePosition) const;
	Path getPath(const sf::Vector2f startPosition, const sf::Vector2f destinationPosition) const;
	sf::Vector2u toNodePosition(sf::Vector2f) const;
	Path getRandomPath(const sf::Vector2f startPosition) const;

private:
	ObstacleGrid mObstacleGrid;
	sf::Vector2f mPlayerPosition;
	const unsigned mSpotSideLength = 16;
	AIMode mAIMode = AIMode::normal;
	bool mHasPlayerMovedSinceLastUpdate = false;
	bool mIsPlayerOnScene = false;
};

} 
