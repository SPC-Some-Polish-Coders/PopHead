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
	Renderer();
	
	Renderer(Renderer&) = delete;
	Renderer& operator=(Renderer&) = delete;

	void beginScene(Camera&);
	void submit(std::shared_ptr<VertexArray>&, std::shared_ptr<Shader>&, const sf::Transform& = sf::Transform::Identity);
	void endScene();

private:
	const float* mViewProjectionMatrix;
};

}
