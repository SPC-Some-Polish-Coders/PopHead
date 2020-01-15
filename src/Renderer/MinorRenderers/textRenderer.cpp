#include "textRenderer.hpp"
#include "Renderer/API/shader.hpp"
#include "Renderer/API/openglErrors.hpp"
#include "Renderer/Shaders/embeddedShaders.hpp"
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

	GLCheck( glGenBuffers(1, &mTextVBO) );
	GLCheck( glBindBuffer(GL_ARRAY_BUFFER, mTextVBO) );
	GLCheck( glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), nullptr, GL_DYNAMIC_DRAW); )

	unsigned indices[] = {0, 1, 2, 2, 3, 0};
	GLCheck( glGenBuffers(1, &mTextIBO) );
	GLCheck( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mTextIBO) );
	GLCheck( glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW) );

	GLCheck( glEnableVertexAttribArray(0) );
	GLCheck( glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0) );
	GLCheck( glEnableVertexAttribArray(1) );
	GLCheck( glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))) );

	// load shader and font
	mTextShader.init(shader::textSrc());
	unsigned uniformBlockIndex = glGetUniformBlockIndex(mTextShader.getID(), "SharedData");
	glUniformBlockBinding(mTextShader.getID(), uniformBlockIndex, 0);
}

void TextRenderer::shutDown()
{
	mFontHolder.clear();
	glDeleteVertexArrays(1, &mTextVAO);
	glDeleteBuffers(1, &mTextVBO);
	glDeleteBuffers(1, &mTextIBO);
	mTextShader.remove();
}

void TextRenderer::drawText(const char* text, const char* fontFilename, sf::Vector2f position, float size, sf::Color color)
{
	SizeSpecificFontData& data = mFontHolder.getSizeSpecificFontData(fontFilename, size);

	GLCheck( glBindVertexArray(mTextVAO) );
	GLCheck( glBindBuffer(GL_ARRAY_BUFFER, mTextVBO) );
	GLCheck( mTextShader.bind() );
	GLCheck( mTextShader.setUniformVector4Color("color", color) );
	GLCheck( glActiveTexture(GL_TEXTURE0) );
	GLCheck( glBindTexture(GL_TEXTURE_2D, data.textureAtlas) );

	while(*text) {
		if(*text >= 32 && *text <= 96) {
			stbtt_aligned_quad q;
			stbtt_GetBakedQuad(data.charactersData, data.textureAtlasSideSize, data.textureAtlasSideSize, *text-32, &position.x, &position.y, &q, 1);
			float vertexData[] = {
				q.x0, q.y0, q.s0, q.t0,
				q.x1, q.y0, q.s1, q.t0,
				q.x1, q.y1, q.s1, q.t1,
				q.x0, q.y1, q.s0, q.t1
			};
			GLCheck( glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexData), vertexData) );
			GLCheck( glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0) );
		}
		++text;
	}
}

}

