#pragma once

#include "vertexArray.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "camera.hpp"
#include <memory>

namespace ph {

class Renderer
{
public:
	static void init();
	
	static void beginScene(Camera&);
	static void submit(std::shared_ptr<VertexArray>&, std::shared_ptr<Shader>&, const sf::Transform& = sf::Transform::Identity);
	static void endScene();

	static void onWindowResize(unsigned width, unsigned height);

private:
	inline static const float* mViewProjectionMatrix = nullptr;
};

}
