#include "pch.hpp"
#include "textRenderer.hpp"
#include "Renderer/renderer.hpp"
#include "quadRenderer.hpp"
#include "Renderer/API/shader.hpp"
#include "Renderer/API/camera.hpp"
#include "Renderer/Shaders/embeddedShaders.hpp"
#include <stb_truetype.h>

namespace ph::TextRenderer {

struct CharacterQuad
{
	IntRect textureRect;
	sf::Vector2f pos;
	sf::Vector2f size;
	float advance;
	sf::Color color;
};

static std::vector<CharacterQuad> rowCharacters;
static FontHolder fontHolder;
static Shader textShader;

static CharacterQuad getCharacterQuad(const stbtt_bakedchar *chardata, int charIndex, sf::Vector2f* pos, int textureWidth)
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

static void drawTextInternal(const char* text, const char* fontFilename, sf::Vector2f position, float fontSize,
                             sf::Color color, unsigned char z, ProjectionType projectionType, bool isAffectedByLight)
{
	SizeSpecificFontData& data = fontHolder.getSizeSpecificFontData(fontFilename, fontSize);

	position.y += fontSize;

	while(*text) {
		if(*text >= '!' && *text <= '~') {
			auto cq = getCharacterQuad(data.charactersData, *text - 32, &position, data.textureAtlas->getWidth());
			Renderer::submitQuad(data.textureAtlas.get(), &cq.textureRect, &color, &textShader, cq.pos, cq.size, z, 0.f, {}, projectionType,
				isAffectedByLight);
			position.x += cq.advance;
		}
		else {
			position.x += fontSize;
		}
		++text;
	}
}

void init()
{
	textShader.init(shader::textSrc());
	textShader.initUniformBlock("SharedData", 0);
	rowCharacters.reserve(100);
}

void shutDown()
{
	textShader.remove();
	fontHolder.clear();
}

void drawText(const char* text, const char* fontFilename, sf::Vector2f pos, float size, sf::Color color,
              unsigned char z, ProjectionType projectionType, bool isAffectedByLight)
{
	drawTextInternal(text, fontFilename, pos, size, color, z, projectionType, isAffectedByLight);
}

void drawTextWorldHD(const char* text, const char* fontFilename, sf::Vector2f worldPos, 
                     const Camera& worldCam, float size, sf::Color textColor, unsigned char z)
{
	auto cameraTopLeft = worldCam.center() - worldCam.getSize() / 2.f; 
	auto pos = (1920 / worldCam.getSize().x) * (worldPos - cameraTopLeft);
	size *= 1920 / worldCam.getSize().x;
	drawTextInternal(text, fontFilename, pos, size, textColor, z, ProjectionType::gui, false);
}

void drawTextArea(const char* text, const char* fontFilename, sf::Vector2f worldPos, float textAreaWidth,
                  TextAligment aligment, float fontSize, sf::Color textColor, unsigned char z, ProjectionType projectionType,
			 	  bool isAffectedByLight)
{
	sf::Color initialTextColor = textColor;

	SizeSpecificFontData& data = fontHolder.getSizeSpecificFontData(fontFilename, fontSize);

	worldPos.y += fontSize;

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
			default: PH_UNEXPECTED_SITUATION(""); return -1.f;
		}
	};

	auto submitCharacterToQuadRenderer = [&](sf::Vector2f localPos, sf::Vector2f size, IntRect textureRect, sf::Color color)
	{
		Renderer::submitQuad(data.textureAtlas.get(), &textureRect, &color, &textShader,
			localPos + worldPos, size, z, 0.f, {}, projectionType, isAffectedByLight);
	};

	while(1)
	{
		if(*text == '@')
		{
			auto getDigit = [](char c) -> float
			{
				// NOTE: stof() doesn't work in this case
				switch(c)
				{
					default:  return 0.f;
					case '1': return 1.f;
					case '2': return 2.f;
					case '3': return 3.f;
					case '4': return 4.f;
					case '5': return 5.f;
					case '6': return 6.f;
					case '7': return 7.f;
					case '8': return 8.f;
					case '9': return 9.f;
				}
			};

			++text;
			switch(*text)
			{
				case 'C': { // change text color
					++text;
					if(*text == 'O')
					{
						textColor = initialTextColor;
					}
					else
					{
						auto getColorValue = [&getDigit, &text] 
						{
							float digit = getDigit(*text);
							return static_cast<unsigned char>((digit / 9.f) * 255.f); 
						};
						textColor.r = getColorValue();
						++text;
						textColor.g = getColorValue();
						++text;
						textColor.b = getColorValue();
						++text;
						textColor.a = getColorValue();
					}
				} break;

				case 'S': { // separate text into 2 columns 
					++text;
					float columnCount = getDigit(*text); 
					++text;
					float currentColumn = getDigit(*text); 
					float textColumnWidth = textAreaWidth / columnCount;
					float currentColumnLeftBound = textColumnWidth * currentColumn;
					if(localPos.x < currentColumnLeftBound)
						localPos.x = currentColumnLeftBound + textColumnWidth / textColumnWidth / 2.f;
					else
						PH_LOG_INFO("Text didn't fit into 2 columns!");
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
		if(*text == 0)
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

