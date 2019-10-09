#pragma once

#include "Vertices/vertexArray.hpp"
#include "Shaders/shaderLibary.hpp"
#include "texture.hpp"
#include "camera.hpp"
#include "sprite.hpp"
#include <SFML/System/Vector2.hpp>
#include <memory>

namespace ph {

class Renderer
{
public:
	static void init();
	
	static void beginScene(Camera&);

	static void submit(VertexArray& vao, Shader& shader, const sf::Transform& transform = sf::Transform::Identity);
	static void submit(VertexArray& vao, const sf::Transform& transform = sf::Transform::Identity);
	static void submit(Sprite&, Shader&, const sf::Transform& = sf::Transform::Identity);
	static void submit(Sprite&, const sf::Transform& = sf::Transform::Identity);
	
	static void endScene();

	static void onWindowResize(unsigned width, unsigned height);

	static void setClearColor(const sf::Color&);

private:
	struct SceneData
	{
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
