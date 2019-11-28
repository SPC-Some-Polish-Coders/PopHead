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

	GLCheck( glGenVertexArrays(1, &mVAO) );
	GLCheck( glBindVertexArray(mVAO) );

	mQuadIBO.bind();

	GLCheck( glGenBuffers(1, &mQuadsDataVBO) );
	GLCheck( glBindBuffer(GL_ARRAY_BUFFER, mQuadsDataVBO) );

	GLCheck( glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(QuadData), (void*) offsetof(QuadData, position)) );
	GLCheck( glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(QuadData), (void*) offsetof(QuadData, size)) );
	GLCheck( glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(QuadData), (void*) offsetof(QuadData, rotation)) );
	GLCheck( glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(QuadData), (void*) offsetof(QuadData, color)) );
	GLCheck( glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(QuadData), (void*) offsetof(QuadData, textureRect)) );
	GLCheck( glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(QuadData), (void*) offsetof(QuadData, textureSlotRef)) );

	for(int i = 0; i < 6; ++i) {
		GLCheck( glEnableVertexAttribArray(i) );
	}
	for(int i = 0; i < 6; ++i) {
		GLCheck( glVertexAttribDivisor(i, 1) );
	}

	mWhiteTexture = new Texture;
	unsigned whiteData = 0xffffffff;
	mWhiteTexture->setData(&whiteData, sizeof(unsigned), sf::Vector2i(1, 1));
}

void QuadRenderer::shutDown()
{
	delete mWhiteTexture;
	mQuadIBO.remove();
	GLCheck( glDeleteBuffers(1, &mQuadsDataVBO) );
	GLCheck( glDeleteVertexArrays(1, &mVAO) );
}

void QuadRenderer::setDebugNumbersToZero()
{
	mNumberOfDrawCalls = 0;
	mNumberOfDrawnSprites = 0;
	mNumberOfDrawnTextures = 0;
	mNumberOfRenderGroups = 0;
}

bool operator< (const RenderGroupKey& lhs, const RenderGroupKey& rhs) {
	if(lhs.shader->getID() < rhs.shader->getID())
		return true;
	if(lhs.z > rhs.z)
		return true;
	return false;
}

void QuadRenderer::submitQuad(const Texture* texture, const IntRect* textureRect, const sf::Color* color, const Shader* shader,
                              sf::Vector2f position, sf::Vector2f size, float z, float rotation)
{
	// culling
	if(!isInsideScreen(position, size, rotation))
		return;

	// if shader is not specified use default shader 
	if(!shader)
		shader = mDefaultInstanedSpriteShader;

	// find or add draw call group
	std::map<RenderGroupKey, QuadRenderGroup>::iterator found;
	while((found = mRenderGroups.find(RenderGroupKey{shader, z})) == mRenderGroups.end())
		mRenderGroups.insert({RenderGroupKey{shader, z}, QuadRenderGroup()});
	auto& renderGroup = found->second;

	// submit data
	QuadData quadData;

	quadData.position = position;
	quadData.size = size;
	quadData.rotation = rotation;
	quadData.color = color ? Cast::toNormalizedColorVector4f(*color) : Cast::toNormalizedColorVector4f(sf::Color::White);
	quadData.textureRect = textureRect ? getNormalizedTextureRect(textureRect, texture->getSize()) : FloatRect(0.f, 0.f, 1.f, 1.f);
	
	if(!texture)
		texture = mWhiteTexture;
	auto textureSlotOfThisTexture = getTextureSlotToWhichThisTextureIsBound(texture, renderGroup);
	if(textureSlotOfThisTexture)
		quadData.textureSlotRef = *textureSlotOfThisTexture;
	else {
		const float textureSlotID = static_cast<float>(renderGroup.textures.size());
		quadData.textureSlotRef = textureSlotID;
		renderGroup.textures.emplace_back(texture);
	}

	renderGroup.quadsData.emplace_back(quadData);
}

bool QuadRenderer::isInsideScreen(sf::Vector2f pos, sf::Vector2f size, float rotation)
{
	if(rotation == 0.f)
		return mScreenBounds->doPositiveRectsIntersect(sf::FloatRect(pos.x, pos.y, size.x, size.y));
	else
		return mScreenBounds->doPositiveRectsIntersect(sf::FloatRect(pos.x - size.x * 2, pos.y - size.y * 2, size.x * 4, size.y * 4));
}


auto QuadRenderer::getTextureSlotToWhichThisTextureIsBound(const Texture* texture, const QuadRenderGroup& rg) -> std::optional<float>
{
	for(size_t i = 0; i < rg.textures.size(); ++i)
		if(rg.textures[i] == texture)
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
	mNumberOfRenderGroups = mRenderGroups.size();

	for(auto& [key, rg] : mRenderGroups)
	{
		// update debug info
		mNumberOfDrawnSprites += rg.quadsData.size();
		mNumberOfDrawnTextures += rg.textures.size();

		// set up shader
		if(key.shader != mCurrentlyBoundShader) 
		{
			key.shader->bind();
			mCurrentlyBoundShader = key.shader;

			int textures[32];
			for(int i = 0; i < 32; ++i)
				textures[i] = i;
			key.shader->setUniformIntArray("textures", 32, textures);
		}
		key.shader->setUniformFloat("z", key.z);

		// sort quads by texture slot ref
		std::sort(rg.quadsData.begin(), rg.quadsData.end(), [](const QuadData& a, const QuadData& b) {
			return a.textureSlotRef < b.textureSlotRef;
		});

		// draw render group
		for(size_t i = 0; i < rg.quadsData.size(); ++i)
		{
			if(i == rg.quadsData.size() - 1)
			{
				bindTexturesForNextDrawCall(rg.textures);
				drawCall(i + 1, rg.quadsData);
				break;
			}
			else if(rg.quadsData[i + 1].textureSlotRef == 32)
			{
				bindTexturesForNextDrawCall(rg.textures);
				drawCall(i + 1, rg.quadsData);

				rg.quadsData.erase(rg.quadsData.begin(), rg.quadsData.begin() + i);
				
				for(QuadData& quadData : rg.quadsData)
					quadData.textureSlotRef -= 32;
			
				rg.textures.erase(
					rg.textures.begin(),
					rg.textures.size() > 32 ? rg.textures.begin() + 32 : rg.textures.end()
				);

				i = 0;
			}
		}
		rg.quadsData.clear();
		rg.textures.clear();
	}
}

void QuadRenderer::bindTexturesForNextDrawCall(std::vector<const Texture*>& textures)
{
	for(size_t i = 0; i < (textures.size() > 32 ? 32 : textures.size()); ++i)
		textures[i]->bind(i);
}

void QuadRenderer::drawCall(unsigned nrOfInstances, std::vector<QuadData>& quadsData)
{
	GLCheck( glBindBuffer(GL_ARRAY_BUFFER, mQuadsDataVBO) );
	GLCheck( glBufferData(GL_ARRAY_BUFFER, nrOfInstances * sizeof(QuadData), quadsData.data(), GL_STATIC_DRAW) );

	GLCheck( glBindVertexArray(mVAO) );
	GLCheck( glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, nrOfInstances) );

	++mNumberOfDrawCalls;
}

}
