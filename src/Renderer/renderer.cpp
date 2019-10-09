#include <GL/glew.h>
#include "renderer.hpp"
#include "GameObjects/gameObject.hpp"
#include "Logs/logs.hpp"
#include "openglErrors.hpp"
#include <SFML/Graphics/Transform.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <array>
#include <iostream>

namespace ph {

void Renderer::init()
{
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK)
		PH_EXIT_GAME("GLEW wasn't initialized correctly!");

	/*GLCheck( const GLubyte* openglVersionInfo = glGetString(GL_VERSION) );
	std::cout << "OpenGL version: " << openglVersionInfo << std::endl;*/

	// set up blending
	GLCheck( glEnable(GL_BLEND) );
	GLCheck( glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) );

	// load default shaders
	auto& sl = ShaderLibrary::getInstance();
	sl.loadFromFile("perfectPixel", "resources/shaders/staticPixelPerfect.vs.glsl", "resources/shaders/texture.fs.glsl");
	sl.loadFromFile("dynamic", "resources/shaders/default.vs.glsl", "resources/shaders/texture.fs.glsl");
	mRendererData.mDefaultShader = sl.get("perfectPixel");
}

void Renderer::beginScene(Camera& camera)
{
	glClear(GL_COLOR_BUFFER_BIT);

	mSceneData.mViewProjectionMatrix = camera.getViewProjectionMatrix4x4().getMatrix();
}

void Renderer::submit(VertexArray& vao, Shader& shader, const sf::Transform& transform, sf::Vector2i size)
{
	vao.bind();

	shader.bind();
	shader.setUniformMatrix4x4("modelMatrix", transform.getMatrix());
	shader.setUniformMatrix4x4("viewProjectionMatrix", mSceneData.mViewProjectionMatrix);

	// TODO: Make possible to draw when index buffer count is not 6
	GLCheck( glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0) );
	
	++mRendererData.mNumberOfDrawCalls;
}

void Renderer::submit(VertexArray& vao, const sf::Transform& transform, sf::Vector2i size)
{
	submit(vao, *mRendererData.mDefaultShader, transform, size);
}

void Renderer::submit(Sprite& sprite, Shader& shader, const sf::Transform& transform)
{
	sprite.mTexture.bind();
	submit(sprite.mVertexArray, shader, transform, sprite.mSize);
}

void Renderer::submit(Sprite& sprite, const sf::Transform& transform)
{
	submit(sprite, *mRendererData.mDefaultShader, transform);
}

void Renderer::isInsideScreen()
{

}

void Renderer::endScene()
{
	std::cout << "DCPF: " << mRendererData.mNumberOfDrawCalls << std::endl;
	mRendererData.mNumberOfDrawCalls = 0;
}

void Renderer::onWindowResize(unsigned width, unsigned height)
{
	GLCheck( glViewport(0, 0, width, height) );
}

void Renderer::setClearColor(const sf::Color& color)
{
	GLCheck( glClearColor(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f) );
}

}
