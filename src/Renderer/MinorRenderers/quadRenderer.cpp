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

	glGenBuffers(1, &mInstancedQuadsDataVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mInstancedQuadsDataVBO);
	glBufferData(GL_ARRAY_BUFFER, mNrOfSpritesInOneInstancedDrawCall * sizeof(QuadData), nullptr, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(QuadData), (void*) offsetof(QuadData, position));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(QuadData), (void*) offsetof(QuadData, size));
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(QuadData), (void*) offsetof(QuadData, rotation));
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(QuadData), (void*) offsetof(QuadData, color));
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(QuadData), (void*) offsetof(QuadData, textureRect));

	for(int i = 0; i < 5; ++i)
		glEnableVertexAttribArray(i);

	for(int i = 0; i < 5; ++i)
		glVertexAttribDivisor(i, 1);

	mWhiteTexture = new Texture;
	unsigned whiteData = 0xffffffff;
	mWhiteTexture->setData(&whiteData, sizeof(unsigned), sf::Vector2i(1, 1));

	// allocate instanced vectors
	mInstancedQuadsData.reserve(mNrOfSpritesInOneInstancedDrawCall);
	mInstancedSpritesTextureSlotRefs.reserve(mNrOfSpritesInOneInstancedDrawCall);
	mInstancedTextures.reserve(32);

	mDefaultInstanedSpriteShader->bind();
	int textures[32];
	for(int i = 0; i < 32; ++i)
		textures[i] = i;
	mDefaultInstanedSpriteShader->setUniformIntArray("textures", 32, textures);
}

void QuadRenderer::shutDown()
{
	delete mWhiteTexture;
	mQuadIBO.remove();
	glDeleteBuffers(1, &mInstancedQuadsDataVBO);
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
	if(mInstancedQuadsData.size() == mNrOfSpritesInOneInstancedDrawCall)
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
	QuadData quadData;
	quadData.position = position;
	quadData.size = size;
	quadData.rotation = rotation;
	quadData.color = color ? Cast::toNormalizedColorVector4f(*color) : Cast::toNormalizedColorVector4f(sf::Color::White);
	quadData.textureRect = textureRect ? getNormalizedTextureRect(textureRect, size) : FloatRect(0.f, 0.f, 1.f, 1.f);
	mInstancedQuadsData.emplace_back(quadData);
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

	glBindBuffer(GL_ARRAY_BUFFER, mInstancedQuadsDataVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, mInstancedQuadsData.size() * sizeof(QuadData), mInstancedQuadsData.data());

	mDefaultInstanedSpriteShader->setUniformIntArray("textureSlotRefs", mInstancedSpritesTextureSlotRefs.size(), mInstancedSpritesTextureSlotRefs.data());

	glBindVertexArray(mInstancedVAO);
	GLCheck( glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, mInstancedQuadsData.size()) );

	++mNumberOfDrawCalls;
	mNumberOfDrawnSprites += mInstancedQuadsData.size();
	mNumberOfDrawnTextures += mInstancedTextures.size();

	mInstancedQuadsData.clear();
	mInstancedSpritesTextureSlotRefs.clear();
	mInstancedTextures.clear();
}

}
