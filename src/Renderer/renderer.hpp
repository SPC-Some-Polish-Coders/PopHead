#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <map>

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
	unsigned shaderProgram, vao;
};

}
