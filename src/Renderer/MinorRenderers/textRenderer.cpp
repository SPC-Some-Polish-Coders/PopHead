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
#include <vector>
#include <array>
#include <cmath>

namespace ph {

namespace {
	struct CharacterQuad
	{
		IntRect textureRect;
		sf::Vector2f pos;
		sf::Vector2f size;
		float advance;
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
		Renderer::submitQuad(nullptr, nullptr, &sf::Color::Black, nullptr, {1120.f, 0.f}, {500.f, 300.f}, 5, 0.f, {}, ProjectionType::gui);
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
	mDebugTextPosition.y += upMargin;
	drawTextInternal(text, fontFilename, mDebugTextPosition, size, color, ProjectionType::gui);
	mDebugTextPosition.y += downMargin + size;
}

void TextRenderer::drawTextArea(const char* text, const char* fontFilename, sf::Vector2f worldPos, const float textAreaWidth,
                                TextAligment aligment, float size, sf::Color color, ProjectionType projectionType)
{
	SizeSpecificFontData& data = mFontHolder.getSizeSpecificFontData(fontFilename, size);

	worldPos.y += size;

	std::vector<CharacterQuad> rowCharacters;
	sf::Vector2f localPos;
	unsigned wordsInCurrentRow = 0;
	unsigned lettersInCurrentWord = 0;

	auto getXOffset = [textAreaWidth, aligment, size](float mostRightCharacterPosX)
	{
		switch(aligment)
		{
			case ph::TextAligment::left: return 0.f;
			case ph::TextAligment::center: return (textAreaWidth - mostRightCharacterPosX) / 2.f;
			case ph::TextAligment::right: return textAreaWidth - mostRightCharacterPosX + size;
			default: break;
		}
	};

	while(1)
	{
		if(*text >= '!' && *text <= '~') 
		{
			auto cq = getCharacterQuad(data.charactersData, *text - 32, &localPos, data.textureAtlas->getWidth());
			localPos.x += cq.advance;
			rowCharacters.emplace_back(cq);
			++lettersInCurrentWord;
		}
		else //probably space character     // TODO: What with double/triple spaces
		{
			localPos.x += size;
			if(localPos.x > textAreaWidth) 
			{
				if(wordsInCurrentRow > 0)
				{
					auto firstNotFittingLetterInTheRow = rowCharacters.begin() + rowCharacters.size() - lettersInCurrentWord;
					const float xOffset = getXOffset(firstNotFittingLetterInTheRow->pos.x);
					for(auto it = rowCharacters.begin(); it != firstNotFittingLetterInTheRow; ++it) 
					{
						it->pos.x += xOffset;
						// TODO: Handle z
						Renderer::submitQuad(data.textureAtlas.get(), &it->textureRect, &color, &mTextShader,
							it->pos + worldPos, it->size, 0, 0.f, {}, projectionType);
					}
					rowCharacters.clear();
					text -= lettersInCurrentWord + 1;
				}
				else
				{
					const float xOffset = getXOffset(localPos.x);
					for(auto& cq : rowCharacters)
					{
						cq.pos.x += xOffset;
						// TODO: Handle z
						Renderer::submitQuad(data.textureAtlas.get(), &cq.textureRect, &color, &mTextShader,
							cq.pos + worldPos, cq.size, 0, 0.f, {}, projectionType);
					}
					rowCharacters.clear();
				}
				localPos.x = 0;
				localPos.y += size;
				wordsInCurrentRow = 0;
			}
			else 
			{
				++wordsInCurrentRow;
			}
			lettersInCurrentWord = 0;
		}

		text++;

		if(*(text) == '\0')
		{
			if(!rowCharacters.empty())
			{
				const float xOffset = getXOffset(localPos.x);
				for(auto& cq : rowCharacters)
				{
					cq.pos.x += xOffset;
					// TODO: Handle z
					Renderer::submitQuad(data.textureAtlas.get(), &cq.textureRect, &color, &mTextShader,
						cq.pos + worldPos, cq.size, 0, 0.f, {}, projectionType);
				}
				rowCharacters.clear();
			}
			break;
		}
	}
}

void TextRenderer::drawTextInternal(const char* text, const char* fontFilename, sf::Vector2f position, float size,
                                    sf::Color color, ProjectionType projectionType)
{
	SizeSpecificFontData& data = mFontHolder.getSizeSpecificFontData(fontFilename, size);

	position.y += size;

	while(*text) {
		if(*text >= '!' && *text <= '~') {
			auto cq = getCharacterQuad(data.charactersData, *text - 32, &position, data.textureAtlas->getWidth());
			// TODO: Handle z
			Renderer::submitQuad(data.textureAtlas.get(), &cq.textureRect, &color, &mTextShader, cq.pos, cq.size, 0, 0.f, {}, projectionType);
			position.x += cq.advance;
		}
		else {
			position.x += size;
		}
		++text;
	}
}

}

