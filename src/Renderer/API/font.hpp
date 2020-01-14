#pragma once

#include <vector>
#include <stb_truetype.h>

namespace ph {

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

}

