#pragma once 

#include <SFML/Graphics.hpp>
#include <vector>
#include <deque>

namespace ph { 

enum class Direction {east, west, north, south, northEast, northWest, southEast, southWest};

class AIManager
{
public:
	AIManager();

	std::deque<Direction> getZombiePath(const sf::Vector2f zombiePosition) const;
	void setPlayerPosition(const sf::Vector2f playerPosition) { this->mPlayerPosition = playerPosition; }

private:
	bool doesZombieSeePlayer(const sf::Vector2f zombiePosition) const;
	std::deque<Direction> getPath(const sf::Vector2f startPosition, const sf::Vector2f destination) const;
	std::deque<Direction> getRandomPath(const sf::Vector2f startPosition) const;

private:
	enum class AreaType { walkable, obstacle };
	std::vector<sf::FloatRect> mOpponentsPositions;
	std::vector<std::vector<AreaType>> mGrid;
	sf::Vector2f mPlayerPosition;
};

} 
