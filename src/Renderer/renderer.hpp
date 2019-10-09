#pragma once

#include "Vertices/vertexArray.hpp"
#include "Shaders/shaderLibary.hpp"
#include "texture.hpp"
#include "camera.hpp"
#include "sprite.hpp"
#include <SFML/System/Vector2.hpp>
#include <Utilities/rect.hpp>
#include <memory>

namespace ph {

class Renderer
{
public:
	static void init();
	
	static void beginScene(Camera&);

	static void submit(VertexArray& vao, Shader& shader, const sf::Transform&, sf::Vector2i size);
	static void submit(VertexArray& vao, const sf::Transform&, sf::Vector2i size);
	static void submit(Sprite&, Shader&, const sf::Transform&);
	static void submit(Sprite&, const sf::Transform&);
	
	static void endScene();

	static void onWindowResize(unsigned width, unsigned height);

	static void setClearColor(const sf::Color&);

private:
	static void isInsideScreen();

private:
	struct SceneData
	{
		const FloatRect mScreenBounds;
		const float* mViewProjectionMatrix = nullptr;
	};

	struct RendererData
	{
		Shader* mDefaultShader;
	};

	inline static SceneData mSceneData;
	inline static RendererData mRendererData;
};

}
