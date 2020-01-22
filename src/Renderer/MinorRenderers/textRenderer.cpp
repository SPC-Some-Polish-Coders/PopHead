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
		sf::Vector2f pos;
		sf::Vector2f size;
		IntRect textureRect;
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
		pos->x += bc->xadvance;
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

void TextRenderer::drawTextArea(const char* text, const char* fontFilename, sf::Vector2f position, float textAreaWidth,
                                TextAligment aligment, float size, sf::Color color, ProjectionType projectionType)
{
	SizeSpecificFontData& data = mFontHolder.getSizeSpecificFontData(fontFilename, size);

	position.y += size;

	std::vector<CharacterQuad> characterQuads;
	characterQuads.reserve(std::strlen(text));
}

void TextRenderer::drawTextInternal(const char* text, const char* fontFilename, sf::Vector2f position, float size,
                                    sf::Color color, ProjectionType projectionType)
{
	SizeSpecificFontData& data = mFontHolder.getSizeSpecificFontData(fontFilename, size);

	position.y += size;

	while(*text) {
		if(*text >= '!' && *text <= '~') {
			auto cq = getCharacterQuad(data.charactersData, *text - 32, &position, data.textureAtlas->getWidth());
			Renderer::submitQuad(data.textureAtlas.get(), &cq.textureRect, &color, &mTextShader, cq.pos, cq.size, 0, 0.f, {}, projectionType);
		}
		else {
			position.x += size;
		}
		++text;
	}
}

}

