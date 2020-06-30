#include "Renderer/API/shader.hpp"
#include "Utilities/vector4.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include "Utilities/rect.hpp"
#include <vector>

namespace ph{

struct PointVertexData
{
	Vec4 color;	
	Vec2 position;
	float size;
};

class PointRenderer
{
public:
	void init();
	void shutDown();

	void setGameWorldCameraBoundsPtr(const FloatRect* cameraBounds) { mGameWorldCameraBounds = cameraBounds; }

	void setDebugCountingActive(bool active) { mIsDebugCountingActive = active; }
	void resetDebugNumbers();

	void submitPoint(Vec2 position, const sf::Color&, float size);

	void flush();

	void submitDebug();

private:
	bool isInsideScreen(Vec2 position, float size);

private:
	std::vector<PointVertexData> mSubmitedPointsVertexData;
	const FloatRect* mGameWorldCameraBounds;
	Shader mPointsShader;
	u32 mVAO;
	u32 mVBO;
	bool mIsDebugCountingActive = false;
};

}

