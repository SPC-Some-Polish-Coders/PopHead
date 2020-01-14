#include "textRenderer.hpp"
#include "Renderer/API/shader.hpp"
#include "Renderer/API/openglErrors.hpp"
#include "Logs/logs.hpp"
#include <GL/glew.h>
#include <cstdio>
#include <cstring>
#include <map>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

namespace ph {

struct SizeSpecificFontData
{
	stbtt_bakedchar charactersData[96];
	int textureAtlasSideSize;
	unsigned textureAtlas;
};

class Font
{
public:
	Font(const char* filename);
	SizeSpecificFontData& getSizeSpecificFontData(float size);

private:
	std::map<float, SizeSpecificFontData> mFontDataMap;
	const char* mFilename;
};

Font::Font(const char* filename)
	:mFilename(filename)
{
}

SizeSpecificFontData& Font::getSizeSpecificFontData(float size)
{
	auto found = mFontDataMap.find(size);
	if(found != mFontDataMap.end()) 
	{
		return found->second;
	}
	else {
		mFontDataMap[size] = SizeSpecificFontData();
		SizeSpecificFontData& data = mFontDataMap[size];

		const int bitmapSideSize = size > 30.f ? size > 70.f ? size > 150.f ? 2048 : 1024 : 512 : 256;
		data.textureAtlasSideSize = bitmapSideSize;

		FILE* file;
		char filepath[50] = "resources/fonts/";
		std::strcat(filepath, mFilename);
		fopen_s(&file, filepath, "rb");
		if(!file)
			PH_EXIT_GAME("Opening font file \" resources/fonts/" + std::string(filepath) + "\" has failed!");

		unsigned char* ttfBuffer = new unsigned char[1 << 20];
		unsigned char* tempBitmap = new unsigned char[bitmapSideSize * bitmapSideSize];

		std::fread(ttfBuffer, 1, 1 << 20, file);
		std::fclose(file);
		stbtt_BakeFontBitmap(ttfBuffer, 0, size, tempBitmap, bitmapSideSize, bitmapSideSize, 32, 96, data.charactersData);

		GLCheck(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
		GLCheck(glGenTextures(1, &data.textureAtlas));
		GLCheck(glBindTexture(GL_TEXTURE_2D, data.textureAtlas));
		GLCheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, bitmapSideSize, bitmapSideSize, 0, GL_RED, GL_UNSIGNED_BYTE, tempBitmap));
		GLCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		delete[] ttfBuffer;
		delete[] tempBitmap;

		return data;
	}
}

namespace {
	Shader* bitmapDebugShader;
	Shader* textShader;
	Font font = Font("joystixMonospace.ttf");
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
}

void drawFontBitmap(const char* filename, float size)
{
	auto& data = font.getSizeSpecificFontData(size);

	glBindVertexArray(fontBitmapVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, data.textureAtlas);
	bitmapDebugShader->bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void drawText(const char* text, const char* fontFilename, sf::Vector2f position, float size, sf::Color color)
{
	auto& data = font.getSizeSpecificFontData(size);

	glBindVertexArray(textVAO);
	glBindBuffer(GL_ARRAY_BUFFER, textVBO);
	textShader->bind();
	textShader->setUniformVector4Color("color", color);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, data.textureAtlas);

	while(*text) {
		// TODO: Is this necessary
		if(*text >= 32 && *text <= 96) {
			stbtt_aligned_quad q;
			stbtt_GetBakedQuad(data.charactersData, data.textureAtlasSideSize, data.textureAtlasSideSize, *text-32, &position.x, &position.y, &q, 1);
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

