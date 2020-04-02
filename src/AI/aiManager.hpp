#pragma once 

#include "pathData.hpp"
#include "obstacleGrid.hpp"
#include <SFML/Graphics.hpp>
#include <deque>

namespace ph { 

enum class AIMode {
	normal,
	zombieAlwaysWalkRandomly,
	zombieAlwaysLookForPlayer
};

struct PathMode
{
	Path path;
	bool isAttackingPath = false;
};

class AIManager
{
public:
	PathMode getZombiePath(sf::Vector2f zombiePosition) const;
	bool shouldZombiePlayAttackAnimation(sf::Vector2f zombiePosition) const;

	bool isPlayerOnScene() const { return mIsPlayerOnScene; }
	void setAIMode(AIMode aiMode) { mAIMode = aiMode; }
	void setIsPlayerOnScene(bool flag) { mIsPlayerOnScene = flag; }
	void setPlayerPosition(sf::Vector2f pos);
	bool hasPlayerMovedSinceLastUpdate() const { return mHasPlayerMovedSinceLastUpdate; }
	void registerTileSize(sf::Vector2f size) { mTileSize = size; }
	void registerMapSize(sf::Vector2u mapSizeInTiles);
	void registerObstacle(sf::Vector2f collisionBodyPosition);

	void update();

private:
	float getDistanceBetweenZombieAndPlayer(sf::Vector2f zombiePosition) const;
	Path getPath(sf::Vector2f startPosition, sf::Vector2f destinationPosition) const;
	sf::Vector2i toNodePosition(sf::Vector2f) const;
	Path getRandomPath(sf::Vector2f startPosition) const;

private:
	ObstacleGrid mObstacleGrid;
	sf::Vector2f mPlayerPosition;
	sf::Vector2f mTileSize = sf::Vector2f(16.f, 16.f);
	AIMode mAIMode = AIMode::normal;
	bool mHasPlayerMovedSinceLastUpdate = false;
	bool mIsPlayerOnScene = false;
};

} 

