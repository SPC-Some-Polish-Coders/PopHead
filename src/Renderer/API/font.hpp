#pragma once

#include "shader.hpp"
#include <vector>
#include <stb_truetype.h>

namespace ph {

	class Shader;

	struct SizeSpecificFontData
	{
		SizeSpecificFontData(const char* filename, float size);

		stbtt_bakedchar charactersData[96];
		int textureAtlasSideSize;
		unsigned textureAtlas;
	};

	class Font
	{
	public:
		Font(const char* filename);
		SizeSpecificFontData& getSizeSpecificFontData(float size);
		std::vector<SizeSpecificFontData>& getFontData() { return mFontData; }
		const char* getFontFilename() const { return mFilename; }
	private:
		std::vector<float> mFontSizes;
		std::vector<SizeSpecificFontData> mFontData;
		const char* mFilename;
	};

	class FontHolder
	{
	public:
		SizeSpecificFontData& getSizeSpecificFontData(const char* filename, float size);
		void clear();
	private:
		std::vector<Font> mFonts;
	};

	class FontDebugRenderer
	{
	public:
		static void init(const char* filename, float size);
		static void shutDown();
		static void draw();
		static bool isActive() { return sData; }
	private:
		struct FontDebugRendererData
		{
			FontDebugRendererData(const char* filename, float size); 
			~FontDebugRendererData();

			SizeSpecificFontData sizeSpecificFontData;
			Shader shader;
			unsigned vao, vbo, ibo;
		};
		inline static FontDebugRendererData* sData = nullptr;
	};

}

