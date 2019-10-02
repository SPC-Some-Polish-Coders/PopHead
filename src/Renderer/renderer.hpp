#pragma once

#include "vertexArray.hpp"
#include <memory>

namespace ph {

class GameObject;
class Map;

class Renderer
{
public:
	Renderer();
	
	Renderer(Renderer&) = delete;
	Renderer& operator=(Renderer&) = delete;

	void setUpModernOpenGlTest();
	void drawModernOpenGlTest();

private:
	unsigned shaderProgram;
	std::shared_ptr<VertexArray> vao;
};

}
