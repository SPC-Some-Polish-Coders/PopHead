#include "textRenderer.hpp"
#include "Renderer/renderer.hpp"
#include "Renderer/API/shader.hpp"
#include "Renderer/API/camera.hpp"
#include "Renderer/API/openglErrors.hpp"
#include "Renderer/Shaders/embeddedShaders.hpp"
#include "quadRenderer.hpp"
#include "Logs/logs.hpp"
#include <GL/glew.h>
#include <stb_truetype.h>
#include <cstdio>
#include <cstring>
#include <map>

namespace ph {

void TextRenderer::init()
{
	// create text vao, vbo, ibo
	GLCheck( glGenVertexArrays(1, &mTextVAO) );
	GLCheck( glBindVertexArray(mTextVAO) );

	GLCheck( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mTextIBO) );

	GLCheck( glGenBuffers(1, &mTextVBO) );
	GLCheck( glBindBuffer(GL_ARRAY_BUFFER, mTextVBO) );
	GLCheck( glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), nullptr, GL_DYNAMIC_DRAW); )

	unsigned quadIndices[] = {0, 1, 2, 2, 3, 0};
	GLCheck( glGenBuffers(1, &mTextIBO) );
	GLCheck( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mTextIBO) );
	GLCheck( glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW) );

	GLCheck( glEnableVertexAttribArray(0) );
	GLCheck( glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0) );
	GLCheck( glEnableVertexAttribArray(1) );
	GLCheck( glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))) );

	// load shaders
	mTextShader.init(shader::textSrc());
	mTextShader.initUniformBlock("SharedData", 0);
}

void TextRenderer::shutDown()
{
	mFontHolder.clear();
	glDeleteVertexArrays(1, &mTextVAO);
	glDeleteBuffers(1, &mTextVBO);
	glDeleteBuffers(1, &mTextIBO);
	mTextShader.remove();
}

void TextRenderer::beginDebugDisplay()
{
	mDebugTextPosition = {1150.f, 10.f};

	if(mWasDebugTextDrawnInLastFrame)
		Renderer::submitQuad(nullptr, nullptr, &sf::Color::Black, nullptr, {1120.f, 10.f}, {500.f, 300.f}, 5, 0.f, {}, ProjectionType::gui);
	mWasDebugTextDrawnInLastFrame = false;
}

void TextRenderer::drawText(const char* text, const char* fontFilename, sf::Vector2f position, float size, sf::Color color,
                            ProjectionType projectionType)
{
	drawTextInternal(text, fontFilename, position, size, color, projectionType);
}

void TextRenderer::drawDebugText(const char* text, const char* fontFilename, float size, float upMargin, float downMargin, sf::Color color)
{
	mWasDebugTextDrawnInLastFrame = true;
	mDebugTextPosition.y += upMargin + size;
	drawTextInternal(text, fontFilename, mDebugTextPosition, size, color, ProjectionType::gui);
	mDebugTextPosition.y += downMargin;
}

void TextRenderer::drawTextInternal(const char* text, const char* fontFilename, sf::Vector2f position, float size,
                                    sf::Color color, ProjectionType projectionType)
{
	SizeSpecificFontData& data = mFontHolder.getSizeSpecificFontData(fontFilename, size);

	mTextShader.bind();
	mTextShader.setUniformVector4Color("color", color);
	mTextShader.setUniformBool("isGameWorldProjection", projectionType == ProjectionType::gameWorld);
	GLCheck( glBindVertexArray(mTextVAO) );
	GLCheck( glBindBuffer(GL_ARRAY_BUFFER, mTextVBO) );
	GLCheck( glActiveTexture(GL_TEXTURE0) );
	GLCheck( glBindTexture(GL_TEXTURE_2D, data.textureAtlas) );

	position.y += size;

	while(*text) {
		if(*text >= '!' && *text <= '~') {
			stbtt_aligned_quad q;
			stbtt_GetBakedQuad(data.charactersData, data.textureAtlasSideSize, data.textureAtlasSideSize,
				*text-32, &position.x, &position.y, &q, 1);
			float vertexData[] = {
				q.x0, q.y0, q.s0, q.t0,
				q.x1, q.y0, q.s1, q.t0,
				q.x1, q.y1, q.s1, q.t1,
				q.x0, q.y1, q.s0, q.t1
			};
			GLCheck( glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexData), vertexData) );
			GLCheck( glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0) );
		}
		else {
			position.x += size;
		}
		++text;
	}
}

}

