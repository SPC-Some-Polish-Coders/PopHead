#include "textRenderer.hpp"
#include "Renderer/renderer.hpp"
#include "quadRenderer.hpp"
#include "Renderer/API/shader.hpp"
#include "Renderer/Shaders/embeddedShaders.hpp"
#include <GL/glew.h>
#include <stb_truetype.h>
#include <cstdio>
#include <cstring>
#include <vector>
#include <cmath>
#include <cstring>

namespace ph {

namespace {
	struct CharacterQuad
	{
		IntRect textureRect;
		sf::Vector2f pos;
		sf::Vector2f size;
		float advance;
		sf::Color color;
	};

	CharacterQuad getCharacterQuad(const stbtt_bakedchar *chardata, int charIndex, sf::Vector2f* pos, int textureWidth)
	{
		const stbtt_bakedchar* bc = chardata + charIndex;
		CharacterQuad q;
		q.pos.x = std::floorf(pos->x + bc->xoff + 0.5f);
		q.pos.y = std::floorf(pos->y + bc->yoff + 0.5f);
		unsigned short width = bc->x1 - bc->x0;
		unsigned short height = bc->y1 - bc->y0;
		q.size = {float(width), float(height)};
		q.textureRect = {bc->x0, textureWidth - bc->y0 - height, width, height};
		q.advance = bc->xadvance;
		return q;
	}
}

void TextRenderer::init()
{
	mTextShader.init(shader::textSrc());
	mTextShader.initUniformBlock("SharedData", 0);
}

void TextRenderer::shutDown()
{
	mTextShader.remove();
	mFontHolder.clear();
}

void TextRenderer::beginDebugDisplay()
{
	mDebugTextPosition = {1150.f, 0.f};

	if(mWasDebugTextDrawnInLastFrame)
		Renderer::submitQuad(nullptr, nullptr, &sf::Color(0, 0, 0, 140), nullptr, {1120.f, 0.f}, {500.f, 300.f},
			5, 0.f, {}, ProjectionType::gui, &sf::Color::White);
	mWasDebugTextDrawnInLastFrame = false;
}

void TextRenderer::drawText(const char* text, const char* fontFilename, sf::Vector2f position, float size, sf::Color color,
                            unsigned char z, ProjectionType projectionType, const sf::Color* localIlluminationColor)
{
	drawTextInternal(text, fontFilename, position, size, color, z, projectionType, localIlluminationColor);
}

void TextRenderer::drawDebugText(const char* text, const char* fontFilename, float size, float upMargin, float downMargin,
                                 sf::Color color, const sf::Color* localIlluminationColor)
{
	mWasDebugTextDrawnInLastFrame = true;
	mDebugTextPosition.y += upMargin;
	drawTextInternal(text, fontFilename, mDebugTextPosition, size, color, 0, ProjectionType::gui, localIlluminationColor);
	mDebugTextPosition.y += downMargin + size;
}

void TextRenderer::drawTextInternal(const char* text, const char* fontFilename, sf::Vector2f position, float fontSize,
                                    sf::Color color, unsigned char z, ProjectionType projectionType, const sf::Color* localIlluminationColor)
{
	SizeSpecificFontData& data = mFontHolder.getSizeSpecificFontData(fontFilename, fontSize);

	position.y += fontSize;

	while(*text) {
		if(*text >= '!' && *text <= '~') {
			auto cq = getCharacterQuad(data.charactersData, *text - 32, &position, data.textureAtlas->getWidth());
			Renderer::submitQuad(data.textureAtlas.get(), &cq.textureRect, &color, &mTextShader, cq.pos, cq.size, z, 0.f, {}, projectionType,
				localIlluminationColor);
			position.x += cq.advance;
		}
		else {
			position.x += fontSize;
		}
		++text;
	}
}

void TextRenderer::drawTextArea(const char* text, const char* fontFilename, sf::Vector2f worldPos, const float textAreaWidth,
                                TextAligment aligment, float fontSize, sf::Color textColor, unsigned char z, ProjectionType projectionType,
								const sf::Color* localIlluminationColor)
{
	SizeSpecificFontData& data = mFontHolder.getSizeSpecificFontData(fontFilename, fontSize);

	worldPos.y += fontSize;

	std::vector<CharacterQuad> rowCharacters;
	sf::Vector2f localPos;
	unsigned wordsInCurrentRow = 0;
	unsigned lettersInCurrentWord = 0;

	auto getXOffset = [textAreaWidth, aligment, fontSize](float mostRightCharacterPosX)
	{
		switch(aligment)
		{
			case ph::TextAligment::left: return 0.f;
			case ph::TextAligment::center: return (textAreaWidth - mostRightCharacterPosX) / 2.f;
			case ph::TextAligment::right: return textAreaWidth - mostRightCharacterPosX + fontSize;
			default: break;
		}
	};

	auto submitCharacterToQuadRenderer = [&](sf::Vector2f localPos, sf::Vector2f size, IntRect textureRect, sf::Color color)
	{
		Renderer::submitQuad(data.textureAtlas.get(), &textureRect, &color, &mTextShader,
			localPos + worldPos, size, z, 0.f, {}, projectionType, localIlluminationColor);
	};

	while(1)
	{
		if(*text == '@')
		{
			++text;
			switch(*text)
			{
				case 'C': { // change text color
					auto getColorValue = [&text] 
					{
						// NOTE: stof() doesn't work in this case
						++text;
						float digit = 0;
						if(*text == '1') digit = 1;
						else if(*text == '2') digit = 2;
						else if(*text == '3') digit = 3;
						else if(*text == '4') digit = 4;
						else if(*text == '5') digit = 5;
						else if(*text == '6') digit = 6;
						else if(*text == '7') digit = 7;
						else if(*text == '8') digit = 8;
						else if(*text == '9') digit = 9;
						return static_cast<unsigned char>((digit / 9.f) * 255.f); 
					};
					textColor.r = getColorValue();
					textColor.g = getColorValue();
					textColor.b = getColorValue();
					textColor.a = getColorValue();
				} break;

				default:
					break;
			}
		}
		else if(*text >= '!' && *text <= '~') 
		{
			auto cq = getCharacterQuad(data.charactersData, *text - 32, &localPos, data.textureAtlas->getWidth());
			localPos.x += cq.advance;
			cq.color = textColor;
			rowCharacters.emplace_back(cq);
			++lettersInCurrentWord;
		}
		else if(*text == ' ' || localPos.x > textAreaWidth)
		{
			while(*(text + 1) == ' ') {
				++text;
				localPos.x += fontSize;
			}
			localPos.x += fontSize;
			if(localPos.x > textAreaWidth) 
			{
				if(wordsInCurrentRow > 0)
				{
					auto firstNotFittingLetterInTheRow = rowCharacters.begin() + rowCharacters.size() - lettersInCurrentWord;
					const float xOffset = getXOffset(firstNotFittingLetterInTheRow->pos.x);
					for(auto it = rowCharacters.begin(); it != firstNotFittingLetterInTheRow; ++it) {
						it->pos.x += xOffset;
						submitCharacterToQuadRenderer(it->pos, it->size, it->textureRect, it->color);
					}
					rowCharacters.clear();
					text -= lettersInCurrentWord + 1;
				}
				else
				{
					const float xOffset = getXOffset(localPos.x);
					for(auto& cq : rowCharacters) {
						cq.pos.x += xOffset;
						submitCharacterToQuadRenderer(cq.pos, cq.size, cq.textureRect, cq.color);
					}
					rowCharacters.clear();
				}
				localPos.x = 0;
				localPos.y += fontSize;
				wordsInCurrentRow = 0;
			}
			else 
			{
				++wordsInCurrentRow;
			}
			lettersInCurrentWord = 0;
		}
		else if(*(text) == '\0')
		{
			if(!rowCharacters.empty())
			{
				const float xOffset = getXOffset(localPos.x);
				for(auto& cq : rowCharacters) {
					cq.pos.x += xOffset;
					submitCharacterToQuadRenderer(cq.pos, cq.size, cq.textureRect, cq.color);
				}
				rowCharacters.clear();
			}
			break;
		}

		++text;
	}
}

}

