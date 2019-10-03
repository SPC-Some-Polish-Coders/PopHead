#include <GL/glew.h>
#include "renderer.hpp"
#include "GameObjects/gameObject.hpp"
#include "Logs/logs.hpp"
#include "Utilities/math.hpp"
#include "Map/map.hpp"
#include "buffers.hpp"
#include "openglErrors.hpp"
#include <array>

namespace ph {

Renderer::Renderer()
{
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK)
		PH_LOG_ERROR("GLEW wasn't initialized correctly!");

	Texture::setDefaultTextureSettings();
}

void Renderer::setUpModernOpenGlTest()
{
	mShader = std::make_shared<Shader>();
	mShader->loadFromFile("resources/shaders/basic.vs.glsl", "resources/shaders/basic.fs.glsl");

	mTexture = std::make_shared<Texture>();
	mTexture->loadFromFile("resources/textures/test/wall.jpg");

	std::array<float, 16> vertices = {
		//positions   textureCoords
		-0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  1.0f, 1.0f
		-0.5f,  0.5f,  0.0f, 1.0f
	};

	std::array<float, 24> positionsWithColors = {
		//positions   
		-0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f
	};

	std::array<unsigned, 6> indices = {
		0, 1, 3,
		1, 2, 3
	};

	VertexBuffer vbo = createVertexBuffer(vertices.data(), vertices.size() * sizeof(float));
	IndexBuffer ibo = createIndexBuffer(indices.data(), indices.size() * sizeof(unsigned));
	mVao = std::make_shared<VertexArray>();
	mVao->setVertexBuffer(vbo, VertexBufferLayout::position2_texCoords2);
	mVao->setIndexBuffer(ibo);
}

void Renderer::drawModernOpenGlTest()
{
	glClearColor(0.2, 0.3, 0.5, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	mTexture->bind();
	mShader->bind();
	mVao->bind();
	GLCheck( glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0) );
}

}
