#pragma once 

#include <SFML/Graphics.hpp>
#include <vector>
#include <deque>

namespace ph { 

class Zombie;
class Object;

enum class Direction {east, west, north, south, northEast, northWest, southEast, southWest};

class AIManager
{
public:
	AIManager();

	std::deque<Direction> whereShouldIgo(const Zombie*);
	void removeOpponentFromManagedObjects(const Object*);
	void setPlayerPosition(const sf::Vector2f playerPosition) { this->mPlayerPosition = playerPosition; }

private:
	std::vector<sf::FloatRect> mOpponentsPositions;
	std::vector<std::vector<bool>> mStaticObjects;
	sf::Vector2f mPlayerPosition;
};

} 
