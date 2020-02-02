#include "font.hpp"
#include "Logs/logs.hpp"
#include "openglErrors.hpp"
#include "Renderer/API/shader.hpp"
#include "Renderer/Shaders/embeddedShaders.hpp"
#include "Logs/logs.hpp"
#include <GL/glew.h>
#include <cstdio>
#include <cstring>
#include <optional>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

namespace ph {

	SizeSpecificFontData::SizeSpecificFontData(const char* filename, float size)
	{
		int textureAtlasSideSize = size > 30.f ? size > 70.f ? size > 150.f ? 2048 : 1024 : 512 : 256;

		FILE* file;
		char filepath[50] = "resources/fonts/";
		strcat_s(filepath, filename);
		fopen_s(&file, filepath, "rb");
		if(!file)
			PH_EXIT_GAME("Opening font file \"" + std::string(filepath) + "\" has failed!");

		unsigned char* ttfBuffer = new unsigned char[1 << 20];
		unsigned char* tempBitmap = new unsigned char[textureAtlasSideSize * textureAtlasSideSize];

		std::fread(ttfBuffer, 1, 1 << 20, file);
		std::fclose(file);
		stbtt_BakeFontBitmap(ttfBuffer, 0, size, tempBitmap, textureAtlasSideSize, textureAtlasSideSize, 32, 96, charactersData);

		unsigned textureAtlasID;
		GLCheck(glGenTextures(1, &textureAtlasID));
		GLCheck(glBindTexture(GL_TEXTURE_2D, textureAtlasID));
		GLCheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, textureAtlasSideSize, textureAtlasSideSize, 0, GL_RED, GL_UNSIGNED_BYTE, tempBitmap));
		GLCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		GLCheck(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
		textureAtlas = std::make_shared<Texture>(textureAtlasID, sf::Vector2i(textureAtlasSideSize, textureAtlasSideSize));

		delete[] ttfBuffer;
		delete[] tempBitmap;
	}

	Font::Font(const char* filename)
		:mFilename(filename)
	{
	}

	SizeSpecificFontData& Font::getSizeSpecificFontData(float size)
	{
		// get if exists
		for(size_t i = 0; i < mFontSizes.size(); ++i)
			if(mFontSizes[i] == size)
				return mFontData[i];

		// create new one if doesn't exist 
		mFontSizes.emplace_back(size);
		return mFontData.emplace_back(mFilename, size);
	}

	SizeSpecificFontData& FontHolder::getSizeSpecificFontData(const char* filename, float size)
	{
		auto findFont = [filename, size, this]() -> Font*
		{
			for(auto& font : mFonts)
				if(std::strcmp(font.getFontFilename(), filename) == 0)
					return &font;
			return nullptr;
		};

		if(Font* foundFont = findFont()) {
			return foundFont->getSizeSpecificFontData(size);
		}
		else {
			auto& newFont = mFonts.emplace_back(filename);
			return newFont.getSizeSpecificFontData(size);
		}
	}

	void FontHolder::clear()
	{
		mFonts.clear();
	}

	FontDebugRenderer::FontDebugRendererData::FontDebugRendererData(const char* filename, float size)
		:sizeSpecificFontData(filename, size)
	{
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		float vertexData[] = {
		   -1.f, -1.f,  0.f,  1.f,
		    1.f, -1.f,  1.f,  1.f,
		    1.f,  1.f,  1.f,  0.f,
		   -1.f,  1.f,  0.f,  0.f
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

		shader.init(shader::fontBitmapDebugSrc());
	}

	FontDebugRenderer::FontDebugRendererData::~FontDebugRendererData()
	{
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ibo);
		glDeleteVertexArrays(1, &vao);
		shader.remove();
	}

	void FontDebugRenderer::init(const char* filename, float size)
	{
		sData = new FontDebugRendererData(filename, size);
	}

	void FontDebugRenderer::shutDown()
	{
		delete sData;
		sData = nullptr;
	}

	void FontDebugRenderer::draw()
	{
		glBindVertexArray(sData->vao);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, sData->sizeSpecificFontData.textureAtlas->getID());
		sData->shader.bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
}

