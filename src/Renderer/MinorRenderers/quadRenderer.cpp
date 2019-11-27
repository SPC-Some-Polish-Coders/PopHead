#include "quadRenderer.hpp"
#include "Renderer/texture.hpp"
#include "Renderer/Shaders/shaderLibary.hpp"
#include "Renderer/openglErrors.hpp"
#include "Utilities/cast.hpp"
#include "Utilities/profiling.hpp"
#include <GL/glew.h>
#include <algorithm>

namespace ph {

void QuadRenderer::init()
{
	auto& sl = ShaderLibrary::getInstance();
	sl.loadFromFile("instancedSprite", "resources/shaders/instancedSprite.vs.glsl", "resources/shaders/instancedSprite.fs.glsl");
	mDefaultInstanedSpriteShader = sl.get("instancedSprite");

	GLCheck( unsigned uniformBlockIndex = glGetUniformBlockIndex(mDefaultInstanedSpriteShader->getID(), "SharedData") );
	GLCheck( glUniformBlockBinding(mDefaultInstanedSpriteShader->getID(), uniformBlockIndex, 0) );

	unsigned quadIndices[] = {0, 1, 3, 1, 2, 3};
	mQuadIBO.init();
	mQuadIBO.setData(quadIndices, sizeof(quadIndices));

	GLCheck( glGenVertexArrays(1, &mInstancedVAO) );
	GLCheck( glBindVertexArray(mInstancedVAO) );

	mQuadIBO.bind();

	GLCheck( glGenBuffers(1, &mInstancedQuadsDataVBO) );
	GLCheck( glBindBuffer(GL_ARRAY_BUFFER, mInstancedQuadsDataVBO) );

	GLCheck( glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(QuadData), (void*) offsetof(QuadData, position)) );
	GLCheck( glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(QuadData), (void*) offsetof(QuadData, size)) );
	GLCheck( glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(QuadData), (void*) offsetof(QuadData, rotation)) );
	GLCheck( glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(QuadData), (void*) offsetof(QuadData, color)) );
	GLCheck( glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(QuadData), (void*) offsetof(QuadData, textureRect)) );
	GLCheck( glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(QuadData), (void*) offsetof(QuadData, textureSlotRef)) );
	GLCheck( glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, sizeof(QuadData), (void*) offsetof(QuadData, z)) );

	for(int i = 0; i < 7; ++i) {
		GLCheck( glEnableVertexAttribArray(i) );
	}
	for(int i = 0; i < 7; ++i) {
		GLCheck( glVertexAttribDivisor(i, 1) );
	}

	mWhiteTexture = new Texture;
	unsigned whiteData = 0xffffffff;
	mWhiteTexture->setData(&whiteData, sizeof(unsigned), sf::Vector2i(1, 1));

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
	GLCheck( glDeleteBuffers(1, &mInstancedQuadsDataVBO) );
	GLCheck( glDeleteVertexArrays(1, &mInstancedVAO) );
}

void QuadRenderer::setDebugNumbersToZero()
{
	mNumberOfDrawCalls = 0;
	mNumberOfDrawnSprites = 0;
	mNumberOfDrawnTextures = 0;
}

// TODO_ren: Support custom shaders for instanced rendering

void QuadRenderer::submitQuad(const Texture* texture, const IntRect* textureRect, const sf::Color* color,
                              sf::Vector2f position, sf::Vector2f size, float z, float rotation)
{
	// culling
	if(!isInsideScreen(position, size, rotation))
		return;

	// find or add draw call group
	std::map<float, QuadDrawCallGroup>::iterator found;
	while((found = mDrawCallGroups.find(z)) == mDrawCallGroups.end())
		mDrawCallGroups.insert({z, QuadDrawCallGroup()});
	auto& drawCallGroup = found->second;

	// submit data
	QuadData quadData;

	quadData.position = position;
	quadData.size = size;
	quadData.rotation = rotation;
	quadData.color = color ? Cast::toNormalizedColorVector4f(*color) : Cast::toNormalizedColorVector4f(sf::Color::White);
	quadData.textureRect = textureRect ? getNormalizedTextureRect(textureRect, texture->getSize()) : FloatRect(0.f, 0.f, 1.f, 1.f);
	quadData.z = z;
	
	if(!texture)
		texture = mWhiteTexture;
	auto textureSlotOfThisTexture = getTextureSlotToWhichThisTextureIsBound(texture, drawCallGroup);
	if(textureSlotOfThisTexture)
		quadData.textureSlotRef = *textureSlotOfThisTexture;
	else {
		const float textureSlotID = static_cast<float>(drawCallGroup.instancedTextures.size());
		quadData.textureSlotRef = textureSlotID;
		drawCallGroup.instancedTextures.emplace_back(texture);
	}

	drawCallGroup.instancedQuadsData.emplace_back(quadData);
}

bool QuadRenderer::isInsideScreen(sf::Vector2f pos, sf::Vector2f size, float rotation)
{
	if(rotation == 0.f)
		return mScreenBounds->doPositiveRectsIntersect(sf::FloatRect(pos.x, pos.y, size.x, size.y));
	else
		return mScreenBounds->doPositiveRectsIntersect(sf::FloatRect(pos.x - size.x * 2, pos.y - size.y * 2, size.x * 4, size.y * 4));
}


auto QuadRenderer::getTextureSlotToWhichThisTextureIsBound(const Texture* texture, const QuadDrawCallGroup& dcg) -> std::optional<float>
{
	for(size_t i = 0; i < dcg.instancedTextures.size(); ++i)
		if(dcg.instancedTextures[i] == texture)
			return static_cast<float>(i);
	return std::nullopt;
}

auto QuadRenderer::getNormalizedTextureRect(const IntRect* pixelTextureRect, sf::Vector2i textureSize) -> FloatRect
{
	auto ts = static_cast<sf::Vector2f>(textureSize);
	return FloatRect(
		pixelTextureRect->left / ts.x, (ts.y - pixelTextureRect->top - pixelTextureRect->height) / ts.y,
		pixelTextureRect->width / ts.x, pixelTextureRect->height / ts.y
	);
}

void QuadRenderer::flush()
{
	PH_PROFILE_FUNCTION();

	mDefaultInstanedSpriteShader->bind();

	for(auto& [z, dcg] : mDrawCallGroups)
	{
		// update debug info
		mNumberOfDrawnSprites += dcg.instancedQuadsData.size();
		mNumberOfDrawnTextures += dcg.instancedTextures.size();

		// subtract 32 from all texture slot refs greater then 31
		for(QuadData& quadData : dcg.instancedQuadsData)
			if(quadData.textureSlotRef > 31)
				quadData.textureSlotRef -= 32;

		// sort by texture slot ref
		std::sort(dcg.instancedQuadsData.begin(), dcg.instancedQuadsData.end(), [](const QuadData& a, const QuadData& b) {
			return a.textureSlotRef < b.textureSlotRef;
		});

		// draw the draw call group
		bindTexturesForNextDrawCall(dcg.instancedTextures);
		for(size_t i = 0; i < dcg.instancedQuadsData.size(); ++i)
		{
			if(i == dcg.instancedQuadsData.size() - 1)
			{
				drawCall(i + 1, dcg.instancedQuadsData);

				dcg.instancedQuadsData.clear();
				dcg.instancedTextures.clear();
				break;
			}
			else if(dcg.instancedQuadsData[i].textureSlotRef == 0 && dcg.instancedQuadsData[i == 0 ? i : i - 1].textureSlotRef == 31)
			{
				drawCall(i + 1, dcg.instancedQuadsData);

				dcg.instancedQuadsData.erase(dcg.instancedQuadsData.begin(), dcg.instancedQuadsData.begin() + i);
			
				dcg.instancedTextures.erase(
					dcg.instancedTextures.begin(),
					dcg.instancedTextures.size() > 32 ? dcg.instancedTextures.begin() + 32 : dcg.instancedTextures.end()
				);
				bindTexturesForNextDrawCall(dcg.instancedTextures);

				i = 0;
			}
		}
	}
}

void QuadRenderer::bindTexturesForNextDrawCall(std::vector<const Texture*>& instancedTextures)
{
	for(size_t i = 0; i < (instancedTextures.size() > 32 ? 32 : instancedTextures.size()); ++i)
		instancedTextures[i]->bind(i);
}

void QuadRenderer::drawCall(unsigned nrOfInstances, std::vector<QuadData>& instancedQuadsData)
{
	GLCheck( glBindBuffer(GL_ARRAY_BUFFER, mInstancedQuadsDataVBO) );
	GLCheck( glBufferData(GL_ARRAY_BUFFER, nrOfInstances * sizeof(QuadData), instancedQuadsData.data(), GL_STATIC_DRAW) );

	GLCheck( glBindVertexArray(mInstancedVAO) );
	GLCheck( glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, nrOfInstances) );

	++mNumberOfDrawCalls;
}

}
