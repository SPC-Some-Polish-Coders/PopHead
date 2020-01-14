#include "textRenderer.hpp"
#include "Renderer/API/shader.hpp"
#include "Renderer/API/openglErrors.hpp"
#include "Logs/logs.hpp"
#include <GL/glew.h>
#include <cstdio>
#include <vector>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

namespace ph {

struct Font
{
	void loadFromFile(const std::string& filepath, int firstChar, int numberOfChars);

	stbtt_bakedchar mCharactersData[96];
	sf::Vector2f bitmapSize;
	int firstChar, numberOfChars;
	unsigned fontTextureAtlas;
};

void Font::loadFromFile(const std::string& filepath, int firstChar, int numberOfChars)
{
	// TODO: Get rid of these
	constexpr float pixelFontHeight = 30;
	constexpr int bitmapWidth = 512;
	constexpr int bitmapHeight = 512;

	this->firstChar = firstChar;
	this->numberOfChars = numberOfChars;
	this->bitmapSize = {bitmapWidth, bitmapHeight};

	FILE* file;
	fopen_s(&file, filepath.c_str(), "rb");
	if(file) {
		unsigned char* ttfBuffer = new unsigned char[1 << 20];
		unsigned char* tempBitmap = new unsigned char[512 * 512];

		std::fread(ttfBuffer, 1, 1 << 20, file);
		std::fclose(file);
		stbtt_BakeFontBitmap(ttfBuffer, 0, pixelFontHeight, tempBitmap, bitmapWidth, bitmapHeight, firstChar, numberOfChars, mCharactersData);

		GLCheck(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
		GLCheck(glGenTextures(1, &fontTextureAtlas));
		GLCheck(glBindTexture(GL_TEXTURE_2D, fontTextureAtlas));
		GLCheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, bitmapWidth, bitmapHeight, 0, GL_RED, GL_UNSIGNED_BYTE, tempBitmap));
		GLCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		delete[] ttfBuffer;
		delete[] tempBitmap;
	}
	else
		PH_EXIT_GAME("Opening font file \"" + filepath + "\" has failed!");
}

namespace {
	Shader* bitmapDebugShader;
	Shader* textShader;
	Font font;
	unsigned fontBitmapVAO, fontBitmapVBO, fontBitmapIBO;
	unsigned textVAO, textVBO, textIBO;
	bool wasInitialized = false;
}

void initTextRenderer()
{
	if(wasInitialized)
		return;
	wasInitialized = true;

	glGenVertexArrays(1, &fontBitmapVAO);
	glBindVertexArray(fontBitmapVAO);

	float vertexData[] = {
		0.f, 0.f, 0.f, 1.f,
		1.f, 0.f, 1.f, 1.f,
		1.f, 1.f, 1.f, 0.f,
		0.f, 1.f, 0.f, 0.f
	};

	// create temporary font bitmap vao, vbo, ibo
	glGenBuffers(1, &fontBitmapVBO);
	glBindBuffer(GL_ARRAY_BUFFER, fontBitmapVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	unsigned indexData[] = {0, 1, 2, 2, 3, 0};
	glGenBuffers(1, &fontBitmapIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fontBitmapIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	// create text vao, vbo, ibo
	glGenVertexArrays(1, &textVAO);
	glBindVertexArray(textVAO);

	glGenBuffers(1, &textVBO);
	glBindBuffer(GL_ARRAY_BUFFER, textVBO);
	glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

	unsigned indices[] = {0, 1, 2, 2, 3, 0};
	glGenBuffers(1, &textIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, textIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	// load shader and font
	bitmapDebugShader = new Shader;
	bitmapDebugShader->loadFromFile("resources/shaders/fontBitmapDebug.vs.glsl", "resources/shaders/fontBitmapDebug.fs.glsl");

	textShader = new Shader;
	textShader->loadFromFile("resources/shaders/text.vs.glsl", "resources/shaders/text.fs.glsl");
	unsigned uniformBlockIndex = glGetUniformBlockIndex(textShader->getID(), "SharedData");
	glUniformBlockBinding(textShader->getID(), uniformBlockIndex, 0);

	font.loadFromFile("resources/fonts/joystixMonospace.ttf", 32, 96);
}

void drawFontBitmap()
{
	glBindVertexArray(fontBitmapVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, font.fontTextureAtlas);
	bitmapDebugShader->bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void drawText(const char* text, sf::Vector2f position, float size, sf::Color color)
{
	// TODO: Handle size
	glBindVertexArray(textVAO);
	glBindBuffer(GL_ARRAY_BUFFER, textVBO);
	textShader->bind();
	textShader->setUniformVector4Color("color", color);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, font.fontTextureAtlas);

	while(*text) {
		// TODO: Is this necessary
		if(*text >= font.firstChar && *text <= font.numberOfChars) {
			stbtt_aligned_quad q;
			stbtt_GetBakedQuad(font.mCharactersData, font.bitmapSize.x, font.bitmapSize.y, *text-32, &position.x, &position.y, &q, 1);
			float vertexData[] = {
				q.x0, q.y0, q.s0, q.t0,
				q.x1, q.y0, q.s1, q.t0,
				q.x1, q.y1, q.s1, q.t1,
				q.x0, q.y1, q.s0, q.t1
			};
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexData), vertexData);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		++text;
	} 
}

}

