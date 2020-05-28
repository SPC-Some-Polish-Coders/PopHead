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
	PathMode getZombiePath(Vec2 zombiePosition) const;
	bool shouldZombiePlayAttackAnimation(Vec2 zombiePosition) const;

	bool isPlayerOnScene() const { return mIsPlayerOnScene; }
	void setAIMode(AIMode aiMode) { mAIMode = aiMode; }
	void setIsPlayerOnScene(bool flag) { mIsPlayerOnScene = flag; }
	void setPlayerPosition(Vec2 pos);
	bool hasPlayerMovedSinceLastUpdate() const { return mHasPlayerMovedSinceLastUpdate; }
	void registerTileSize(Vec2 size) { mTileSize = size; }
	void registerMapSize(Vec2u mapSizeInTiles);
	void registerObstacle(Vec2 collisionBodyPosition);

	void update();

private:
	float getDistanceBetweenZombieAndPlayer(Vec2 zombiePosition) const;
	Path getPath(Vec2 startPosition, Vec2 destinationPosition) const;
	Vec2i toNodePosition(Vec2) const;
	Path getRandomPath(Vec2 startPosition) const;

private:
	ObstacleGrid mObstacleGrid;
	Vec2 mPlayerPosition;
	Vec2 mTileSize = Vec2(16.f, 16.f);
	AIMode mAIMode = AIMode::normal;
	bool mHasPlayerMovedSinceLastUpdate = false;
	bool mIsPlayerOnScene = false;
};

} 

