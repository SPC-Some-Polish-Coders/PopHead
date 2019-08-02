#include "RandomPathAlgorithm.hpp"
#include "Utilities/random.hpp"
#include <algorithm>

namespace ph {

RandomPathAlgorithm::RandomPathAlgorithm(const ObstacleGrid& obstacleGrid)
	:mObstacleGrid(obstacleGrid)
{
}

Path RandomPathAlgorithm::getRandomPath(const sf::Vector2u startNodePosition)
{
	const float lengthOfPath = Random::generateNumber(2, 5);
	Path path;
	path.resize(lengthOfPath);
	Direction direction = static_cast<Direction>(Random::generateNumber(0, 3));
	std::fill(path.begin(), path.end(), direction);
	return path;
}

}
