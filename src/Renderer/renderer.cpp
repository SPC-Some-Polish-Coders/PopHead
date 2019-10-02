#include <GL/glew.h>
#include "renderer.hpp"
#include "GameObjects/gameObject.hpp"
#include "Logs/logs.hpp"
#include "Utilities/math.hpp"
#include "Map/map.hpp"
#include "buffers.hpp"
#include "openglErrors.hpp"

namespace ph {

Renderer::Renderer()
{
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK)
		PH_LOG_ERROR("GLEW wasn't initialized correctly!");
}

void Renderer::setUpModernOpenGlTest()
{
	mShader = std::make_shared<Shader>();
	mShader->loadFromFile("resources/shaders/basic.vs.glsl", "resources/shaders/basic.fs.glsl");

	float vertices[] = {
		-0.5f, -0.5f,
		 0.5f, -0.5f,
		 0.0f,  0.5f
	};

	unsigned int indices[] = {0, 1, 2};

	VertexBuffer vbo = createVertexBuffer(vertices, 3 * 3 * sizeof(float));
	IndexBuffer ibo = createIndexBuffer(indices, 3 * sizeof(unsigned));
	mVao = std::make_shared<VertexArray>();
	mVao->setVertexBuffer(vbo, VertexBufferLayout::position2);
	mVao->setIndexBuffer(ibo);
	mVao->bind();
}

void Renderer::drawModernOpenGlTest()
{
	mShader->bind();
	mVao->bind();
	GLCheck( glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0) );
}

}
