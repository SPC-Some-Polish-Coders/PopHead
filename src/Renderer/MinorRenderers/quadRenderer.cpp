#include "quadRenderer.hpp"
#include "Renderer/texture.hpp"
#include "Renderer/Shaders/shaderLibary.hpp"
#include "Renderer/openglErrors.hpp"
#include "Utilities/cast.hpp"
#include <GL/glew.h>

namespace ph {

void QuadRenderer::init()
{
	auto& sl = ShaderLibrary::getInstance();
	sl.loadFromFile("instancedSprite", "resources/shaders/instancedSprite.vs.glsl", "resources/shaders/instancedSprite.fs.glsl");
	mDefaultInstanedSpriteShader = sl.get("instancedSprite");

	unsigned quadIndices[] = {0, 1, 3, 1, 2, 3};
	mQuadIBO.init();
	mQuadIBO.setData(quadIndices, sizeof(quadIndices));

	// TODO_ren: Try to pack it into one struct

	glGenVertexArrays(1, &mInstancedVAO);
	glBindVertexArray(mInstancedVAO);

	mQuadIBO.bind();

	glGenBuffers(1, &mInstancedPositionsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mInstancedPositionsVBO);
	glBufferData(GL_ARRAY_BUFFER, mNrOfSpritesInOneInstancedDrawCall * sizeof(sf::Vector2f), nullptr, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), ( void*) 0);
	glVertexAttribDivisor(0, 1);

	glGenBuffers(1, &mInstancedSizesVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mInstancedSizesVBO);
	glBufferData(GL_ARRAY_BUFFER, mNrOfSpritesInOneInstancedDrawCall * sizeof(sf::Vector2f), nullptr, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), ( void*) 0);
	glVertexAttribDivisor(1, 1);

	glGenBuffers(1, &mInstancedRotationsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mInstancedRotationsVBO);
	glBufferData(GL_ARRAY_BUFFER, mNrOfSpritesInOneInstancedDrawCall * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(float), ( void*) 0);
	glVertexAttribDivisor(2, 1);

	glGenBuffers(1, &mInstancedColorsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mInstancedColorsVBO);
	glBufferData(GL_ARRAY_BUFFER, mNrOfSpritesInOneInstancedDrawCall * 4 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), ( void*) 0);
	glVertexAttribDivisor(3, 1);

	glGenBuffers(1, &mInstancedTextureRectsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mInstancedTextureRectsVBO);
	glBufferData(GL_ARRAY_BUFFER, mNrOfSpritesInOneInstancedDrawCall * 4 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), ( void*) 0);
	glVertexAttribDivisor(4, 1);

	mWhiteTexture = new Texture;
	unsigned whiteData = 0xffffffff;
	mWhiteTexture->setData(&whiteData, sizeof(unsigned), sf::Vector2i(1, 1));

	// allocate instanced vectors
	mInstancedSpritesPositions.reserve(mNrOfSpritesInOneInstancedDrawCall);
	mInstancedSpritesSizes.reserve(mNrOfSpritesInOneInstancedDrawCall);
	mInstancedSpritesRotations.reserve(mNrOfSpritesInOneInstancedDrawCall);
	mInstancedSpritesColors.reserve(mNrOfSpritesInOneInstancedDrawCall);
	mInstancedSpritesTextureRects.reserve(mNrOfSpritesInOneInstancedDrawCall);
	mInstancedSpritesTextureSlotRefs.reserve(mNrOfSpritesInOneInstancedDrawCall);
	mInstancedTextures.reserve(32);

	// TODO_ren: Optimize that
	mDefaultInstanedSpriteShader->bind();
	for(size_t i = 0; i < 32; ++i)
		mDefaultInstanedSpriteShader->setUniformInt("textures[" + std::to_string(i) + "]", i);
}

void QuadRenderer::shutDown()
{
	delete mWhiteTexture;
	mQuadIBO.remove();
	glDeleteBuffers(1, &mInstancedPositionsVBO);
	glDeleteBuffers(1, &mInstancedSizesVBO);
	glDeleteBuffers(1, &mInstancedRotationsVBO);
	glDeleteBuffers(1, &mInstancedColorsVBO);
	glDeleteBuffers(1, &mInstancedTextureRectsVBO);
	glDeleteVertexArrays(1, &mInstancedVAO);
}

void QuadRenderer::setScreenBoundsPtr(const FloatRect* screenBounds)
{
	mScreenBounds = screenBounds;
}

void QuadRenderer::setViewProjectionMatrix(const float* viewProjectionMatrix)
{
	mViewProjectionMatrix = viewProjectionMatrix;
}

void QuadRenderer::setDebugNumbersToZero()
{
	mNumberOfDrawCalls = 0;
	mNumberOfDrawnSprites = 0;
	mNumberOfDrawnTextures = 0;
}

// TODO_ren: Support custom shaders for instanced rendering

void QuadRenderer::submitQuad(const Texture* texture, const IntRect* textureRect, const sf::Color* color,
                              sf::Vector2f position, sf::Vector2f size, float rotation)
{
	// culling
	if(!isInsideScreen(position, size))
		return;

	// flush if there is too much submited sprites
	if(mInstancedSpritesPositions.size() == mNrOfSpritesInOneInstancedDrawCall)
		flush();

	// submit textures data
	if(!texture)
		texture = mWhiteTexture;
	auto textureSlotOfThisTexture = getTextureSlotToWhichThisTextureIsBound(texture);
	if(textureSlotOfThisTexture)
		mInstancedSpritesTextureSlotRefs.emplace_back(*textureSlotOfThisTexture);
	else {
		if(mInstancedTextures.size() == 32)
			flush();

		const int textureSlotID = static_cast<int>(mInstancedTextures.size());
		mInstancedSpritesTextureSlotRefs.emplace_back(textureSlotID);
		texture->bind(textureSlotID);
		mInstancedTextures.emplace_back(texture);
	}

	// submit rest of data
	mInstancedSpritesPositions.emplace_back(position);
	mInstancedSpritesSizes.emplace_back(size);
	mInstancedSpritesRotations.emplace_back(rotation);
	mInstancedSpritesColors.emplace_back(color ? Cast::toNormalizedColorVector4f(*color) : Cast::toNormalizedColorVector4f(sf::Color::White));
	mInstancedSpritesTextureRects.emplace_back(textureRect ? getNormalizedTextureRect(textureRect, size) : FloatRect(0.f, 0.f, 1.f, 1.f));
}

bool QuadRenderer::isInsideScreen(sf::Vector2f position, sf::Vector2f size)
{
	return isInsideScreen(sf::FloatRect(position.x, position.y, size.x, size.y));
}

bool QuadRenderer::isInsideScreen(const FloatRect objectBounds)
{
	return mScreenBounds->doPositiveRectsIntersect(objectBounds);
}

auto QuadRenderer::getTextureSlotToWhichThisTextureIsBound(const Texture* texture) -> std::optional<int>
{
	for(size_t i = 0; i < mInstancedTextures.size(); ++i)
		if(mInstancedTextures[i] == texture)
			return i;
	return std::nullopt;
}

auto QuadRenderer::getNormalizedTextureRect(const IntRect* pixelTextureRect, sf::Vector2f size) -> FloatRect
{
	return FloatRect(
		pixelTextureRect->left / size.x, pixelTextureRect->top / size.y,
		pixelTextureRect->width / size.x, pixelTextureRect->height / size.y
	);
}

void QuadRenderer::flush()
{
	mDefaultInstanedSpriteShader->bind();
	mDefaultInstanedSpriteShader->setUniformMatrix4x4("viewProjectionMatrix", mViewProjectionMatrix);

	glBindBuffer(GL_ARRAY_BUFFER, mInstancedPositionsVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, mInstancedSpritesPositions.size() * sizeof(sf::Vector2f), mInstancedSpritesPositions.data());

	glBindBuffer(GL_ARRAY_BUFFER, mInstancedSizesVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, mInstancedSpritesSizes.size() * sizeof(sf::Vector2f), mInstancedSpritesSizes.data());

	glBindBuffer(GL_ARRAY_BUFFER, mInstancedRotationsVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, mInstancedSpritesRotations.size() * sizeof(float), mInstancedSpritesRotations.data());

	glBindBuffer(GL_ARRAY_BUFFER, mInstancedColorsVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, mInstancedSpritesColors.size() * 4 * sizeof(float), mInstancedSpritesColors.data());

	glBindBuffer(GL_ARRAY_BUFFER, mInstancedTextureRectsVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, mInstancedSpritesTextureRects.size() * 4 * sizeof(float), mInstancedSpritesTextureRects.data());

	for(size_t i = 0; i < mInstancedSpritesTextureSlotRefs.size(); ++i)
		mDefaultInstanedSpriteShader->setUniformInt("textureSlotRefs[" + std::to_string(i) + "]", mInstancedSpritesTextureSlotRefs[i]);

	glBindVertexArray(mInstancedVAO);
	GLCheck( glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, mInstancedSpritesPositions.size()) );

	++mNumberOfDrawCalls;
	mNumberOfDrawnSprites += mInstancedSpritesPositions.size();
	mNumberOfDrawnTextures += mInstancedTextures.size();

	mInstancedSpritesPositions.clear();
	mInstancedSpritesSizes.clear();
	mInstancedSpritesRotations.clear();
	mInstancedSpritesColors.clear();
	mInstancedSpritesTextureRects.clear();
	mInstancedSpritesTextureSlotRefs.clear();
	mInstancedTextures.clear();
}

}
