#include "Renderer/API/shader.hpp"
#include "Utilities/vector4.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include "Utilities/rect.hpp"
#include <vector>

namespace ph{

struct PointVertexData
{
	Vector4f color;	
	sf::Vector2f position;
	float size;
	float z;
};

class PointRenderer
{
public:
	void init();
	void shutDown();

	void setScreenBoundsPtr(const FloatRect* screenBounds) { mScreenBounds = screenBounds; }

	unsigned getNrOfDrawnPoints() const { return mNrOfDrawnPoints; }
	unsigned getNrOfDrawCalls() const { return mNrOfDrawCalls; }
	void setDebugNumbersToZero();

	void submitPoint(sf::Vector2f position, const sf::Color&, float z, float size);

	void flush();

private:
	bool isInsideScreen(sf::Vector2f position, float size);

private:
	std::vector<PointVertexData> mSubmitedPointsVertexData;
	const FloatRect* mScreenBounds;
	Shader mPointsShader;
	unsigned mVAO;
	unsigned mVBO;
	unsigned mNrOfDrawnPoints;
	unsigned mNrOfDrawCalls;
};

}

