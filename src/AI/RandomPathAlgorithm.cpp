#include "RandomPathAlgorithm.hpp"
#include "Utilities/random.hpp"

namespace ph {

RandomPathAlgorithm::RandomPathAlgorithm(const ObstacleGrid& obstacleGrid)
	:mObstacleGrid(obstacleGrid)
{
}

Path RandomPathAlgorithm::getRandomPath(const sf::Vector2u startNodePosition)
{
	return Path();
}

}
