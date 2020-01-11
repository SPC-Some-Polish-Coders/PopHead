#include "textRenderer.hpp"
#include "Renderer/API/shader.hpp"
#include <cstdio>
#include <GL/glew.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

namespace ph {

class Font
{
public:
	void loadFromFile(const std::string& filepath);

private:
	stbtt_bakedchar mCharactersData[96];
	unsigned fontTextureAtlas;

	friend void drawFontBitmap();
};

void Font::loadFromFile(const std::string& filepath)
{
	// TODO: Get rid of these
	constexpr float pixelFontHeight = 40;
	constexpr int bitmapWidth = 512;
	constexpr int bitmapHeight = 512;
	constexpr int firstChar = 32;
	constexpr int numberOfChars = 96;

	unsigned char* ttfBuffer = new unsigned char[1 << 20];
	unsigned char* tempBitmap = new unsigned char[512 * 512];

	std::fread(ttfBuffer, 1, 1 << 20, std::fopen(filepath.c_str(), "rb"));
	stbtt_BakeFontBitmap(ttfBuffer, 0, pixelFontHeight, tempBitmap, bitmapWidth, bitmapHeight, firstChar, numberOfChars, mCharactersData);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &fontTextureAtlas);
	glBindTexture(GL_TEXTURE_2D, fontTextureAtlas);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, bitmapWidth, bitmapHeight, 0, GL_RED, GL_UNSIGNED_BYTE, tempBitmap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	delete[] ttfBuffer;
	delete[] tempBitmap;
}

namespace {
	Shader* textShader;
	Font font;
	unsigned vao, vbo, ibo;
	bool wasInitialized = false;
}

void initFontBitmap()
{
	if(wasInitialized)
		return;
	wasInitialized = true;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	float vertexData[] = {
		0.f, 0.f, 0.f, 1.f,
		1.f, 0.f, 1.f, 1.f,
		1.f, 1.f, 1.f, 0.f,
		0.f, 1.f, 0.f, 0.f
	};

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	unsigned indexData[] = {0, 1, 2, 2, 3, 0};
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	textShader = new Shader;
	textShader->loadFromFile("resources/shaders/text.vs.glsl", "resources/shaders/text.fs.glsl");
	font.loadFromFile("resources/fonts/joystixMonospace.ttf");
}

void drawFontBitmap()
{
	glBindVertexArray(vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, font.fontTextureAtlas);
	textShader->bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

}

