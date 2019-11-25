#include "Utilities/vector4.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <vector>

namespace ph{

class Shader;

struct PointVertexData
{
	Vector4f color;	
	sf::Vector2f position;
	float size;
};

class PointRenderer
{
public:
	void init();
	void shutDown();

	unsigned getNrOfDrawnPoints() const { return mNrOfDrawnPoints; }
	unsigned getNrOfDrawCalls() const { return mNrOfDrawCalls; }
	void setDebugNumbersToZero();

	void submitPoint(sf::Vector2f position, const sf::Color&, float size);

	void flush();

private:
	std::vector<PointVertexData> mSubmitedPointsVertexData;
	Shader* mPointsShader;
	unsigned mVAO;
	unsigned mVBO;
	unsigned mNrOfDrawnPoints;
	unsigned mNrOfDrawCalls;
};

}
