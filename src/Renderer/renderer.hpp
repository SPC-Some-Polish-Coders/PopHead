#pragma once

#include "Vertices/vertexArray.hpp"
#include "Shaders/shaderLibary.hpp"
#include "texture.hpp"
#include "camera.hpp"
#include "sprite.hpp"
#include "glEnums.hpp"
#include "SFMLrenderer.hpp"
#include "Utilities/rect.hpp"
#include <SFML/System/Vector2.hpp>
#include <memory>

namespace ph {

class Renderer
{
public:
	static void init();
	
	static void beginScene(Camera&);

	static void submit(VertexArray& vao, Shader& shader, const sf::Transform&, const sf::Vector2i size, DrawPrimitive = DrawPrimitive::Triangles);
	static void submit(VertexArray& vao, Shader& shader, const FloatRect bounds, DrawPrimitive = DrawPrimitive::Triangles);
	static void submit(VertexArray& vao, const FloatRect bounds, DrawPrimitive = DrawPrimitive::Triangles);
	static void submit(VertexArray& vao, const sf::Transform&, const sf::Vector2i size, DrawPrimitive = DrawPrimitive::Triangles);
	static void submit(Sprite&, Shader&, const sf::Transform&, DrawPrimitive = DrawPrimitive::Triangles);
	static void submit(Sprite&, const sf::Transform&, DrawPrimitive = DrawPrimitive::Triangles);
	static void submit(const sf::Drawable&);

	static void endScene(sf::RenderWindow& window);

	static void onWindowResize(unsigned width, unsigned height);

	static void setClearColor(const sf::Color&);

private:
	static bool isInsideScreen(const sf::Transform&, const sf::Vector2i size);
	static bool isInsideScreen(const FloatRect objectBounds);

private:
	struct SceneData
	{
		FloatRect mScreenBounds;
		const float* mViewProjectionMatrix = nullptr;
	};

	struct RendererData
	{
		unsigned mNumberOfDrawCalls = 0;
		Shader* mDefaultShader;
	};

	inline static SceneData mSceneData;
	inline static RendererData mRendererData;

	// TODO: Get rid of SFML Renderer
	inline static SFMLRenderer mSFMLRenderer;
};

}
