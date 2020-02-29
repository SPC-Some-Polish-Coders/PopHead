#include "quadRenderer.hpp"
#include "Renderer/API/texture.hpp"
#include "Renderer/API/openglErrors.hpp"
#include "Renderer/Shaders/embeddedShaders.hpp"
#include "Utilities/cast.hpp"
#include "Utilities/profiling.hpp"
#include "Utilities/math.hpp"
#include "Utilities/memory.hpp"
#include <GL/glew.h>
#include <algorithm>

namespace ph {

RenderGroupsHashMap::RenderGroupsHashMap()
	:mShouldSort(false)
{
	mRenderGroups.reserve(10);
}

QuadRenderGroup& RenderGroupsHashMap::insertIfDoesNotExistAndGetRenderGroup(RenderGroupKey key) 
{
	if(auto renderGroup = getRenderGroup(key))
		return *renderGroup;
	mRenderGroups.emplace_back(std::pair(key, QuadRenderGroup()));
	mShouldSort = true;
	return *getRenderGroup(key);
}

auto RenderGroupsHashMap::getUnderlyingVector() -> std::vector<std::pair<RenderGroupKey, QuadRenderGroup>>&
{
	eraseUselessGroups();
	sort();
	return mRenderGroups;
}

void RenderGroupsHashMap::sort()
{
	if(!mShouldSort)
		return;

	// TODO_ren: Make more smart sorting so we don't need to rebind shaders that often
	//           Use the fact that we don't need to sort everything by z because not every quad is transparent

	std::sort(mRenderGroups.begin(), mRenderGroups.end(), []
	(const std::pair<RenderGroupKey, QuadRenderGroup>& a, std::pair<RenderGroupKey, QuadRenderGroup>& b) {
		return a.first.z > b.first.z;
	});
}

void RenderGroupsHashMap::eraseUselessGroups()
{
	for(size_t i = 0; i < mRenderGroups.size(); ++i)
		if(mRenderGroups[i].second.quadsData.empty())
			mRenderGroups.erase(mRenderGroups.begin() + i);
}

QuadRenderGroup* RenderGroupsHashMap::getRenderGroup(RenderGroupKey key)
{
	for(size_t i = 0; i < mRenderGroups.size(); ++i)
		if(mRenderGroups[i].first == key)
			return &mRenderGroups[i].second;
	return nullptr;
}

bool operator==(const RenderGroupKey& lhs, const RenderGroupKey& rhs)
{
	return lhs.shader == rhs.shader && lhs.z == rhs.z;
}

void QuadRenderer::init()
{
	mDefaultQuadShader.init(shader::quadSrc());
	mDefaultQuadShader.initUniformBlock("SharedData", 0);

	GLCheck( glGenVertexArrays(1, &mVAO) );
	GLCheck( glBindVertexArray(mVAO) );

	unsigned quadIndices[] = {0, 1, 3, 1, 2, 3};
	GLCheck( glGenBuffers(1, &mQuadIBO) );
	GLCheck( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mQuadIBO) );
	GLCheck( glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW) ); 

	GLCheck( glGenBuffers(1, &mQuadsDataVBO) );
	GLCheck( glBindBuffer(GL_ARRAY_BUFFER, mQuadsDataVBO) );

	GLCheck( glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(QuadData), (void*) offsetof(QuadData, color)) );
	GLCheck( glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(QuadData), (void*) offsetof(QuadData, textureRect)) );
	GLCheck( glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(QuadData), (void*) offsetof(QuadData, position)) );
	GLCheck( glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(QuadData), (void*) offsetof(QuadData, size)) );
	GLCheck( glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(QuadData), (void*) offsetof(QuadData, rotationOrigin)) );
	GLCheck( glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(QuadData), (void*) offsetof(QuadData, rotation)) );
	GLCheck( glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, sizeof(QuadData), (void*) offsetof(QuadData, textureSlotRef)) );

	for(int i = 0; i < 7; ++i) {
		GLCheck( glEnableVertexAttribArray(i) );
	}
	for(int i = 0; i < 7; ++i) {
		GLCheck( glVertexAttribDivisor(i, 1) );
	}

	mWhiteTexture = new Texture;
	unsigned whiteData = 0xffffffff;
	mWhiteTexture->setData(&whiteData, sizeof(unsigned), sf::Vector2i(1, 1));
}

void QuadRenderer::shutDown()
{
	delete mWhiteTexture;
	mDefaultQuadShader.remove();
	GLCheck( glDeleteBuffers(1, &mQuadIBO) );
	GLCheck( glDeleteBuffers(1, &mQuadsDataVBO) );
	GLCheck( glDeleteVertexArrays(1, &mVAO) );
}

void QuadRenderer::resetDebugNumbers()
{
	mDebugNumbers = {};
}

void QuadRenderer::submitBunchOfQuadsWithTheSameTexture(std::vector<QuadData>& quadsData, const Texture* texture,
                                                        const Shader* shader, float z, ProjectionType projectionType)
{
	if(!shader)
		shader = &mDefaultQuadShader;

	auto& renderGroup = mRenderGroupsHashMap.insertIfDoesNotExistAndGetRenderGroup({shader, z, projectionType});
	
	if(!texture)
		texture = mWhiteTexture;
	auto textureSlotOfThisTexture = getTextureSlotToWhichThisTextureIsBound(texture, renderGroup);
	if(textureSlotOfThisTexture) {
		for(auto& quad : quadsData)
			quad.textureSlotRef = *textureSlotOfThisTexture;
	}
	else {
		const float textureSlotID = static_cast<float>(renderGroup.textures.size());
		for(auto& quad : quadsData)
			quad.textureSlotRef = textureSlotID;
		renderGroup.textures.emplace_back(texture);
	}

	renderGroup.quadsData.insert(renderGroup.quadsData.end(), quadsData.begin(), quadsData.end());
}

void QuadRenderer::submitQuad(const Texture* texture, const IntRect* textureRect, const sf::Color* color, const Shader* shader,
                              sf::Vector2f position, sf::Vector2f size, float z, float rotation, sf::Vector2f rotationOrigin,
                              ProjectionType projectionType, bool isAffectedByLight)
{
	// culling
	FloatRect bounds = projectionType == ProjectionType::gameWorld ? *mScreenBounds : FloatRect(0.f, 0.f, 1920.f, 1080.f);
	if(rotation == 0.f)
		if(!bounds.doPositiveRectsIntersect(FloatRect(position.x, position.y, size.x, size.y)))
			return;
	else if(!bounds.doPositiveRectsIntersect(FloatRect(position.x - size.x * 2, position.y - size.y * 2, size.x * 4, size.y * 4)))
		return;

	// if shader is not specified use default shader 
	if(!shader)
		shader = &mDefaultQuadShader;

	// find or add draw call group
	auto& renderGroupsHashMap = isAffectedByLight ? mRenderGroupsHashMap : mNotAffectedByLightRenderGroupsHashMap;
	auto& renderGroup = renderGroupsHashMap.insertIfDoesNotExistAndGetRenderGroup(RenderGroupKey{shader, z, projectionType});

	// submit data
	QuadData quadData;

	quadData.color = color ? Cast::toNormalizedColorVector4f(*color) : Cast::toNormalizedColorVector4f(sf::Color::White);
	quadData.textureRect = textureRect ? getNormalizedTextureRect(textureRect, texture->getSize()) : FloatRect(0.f, 0.f, 1.f, 1.f);
	quadData.position = position;
	quadData.size = size;
	quadData.rotationOrigin = rotationOrigin;
	quadData.rotation = Math::degreesToRadians(rotation);
	
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

void QuadRenderer::flush(bool affectedByLight)
{
	PH_PROFILE_FUNCTION(0);

	if(affectedByLight)
		mDebugNumbers.renderGroups = static_cast<unsigned>(mRenderGroupsHashMap.size());
	else
		mDebugNumbers.renderGroupsNotAffectedByLight = static_cast<unsigned>(mNotAffectedByLightRenderGroupsHashMap.size());
		

	mCurrentlyBoundQuadShader = nullptr;
	auto& renderGroupsHashMap = affectedByLight ? mRenderGroupsHashMap : mNotAffectedByLightRenderGroupsHashMap;

	for(auto& [key, rg] : renderGroupsHashMap.getUnderlyingVector())
	{
		// update debug info
		if(mIsDebugCountingActive) {
			mDebugNumbers.drawnSprites += static_cast<unsigned>(rg.quadsData.size());
			mDebugNumbers.drawnTextures += static_cast<unsigned>(rg.textures.size());
			if(affectedByLight)
				mDebugNumbers.renderGroupsSizes.data[mDebugNumbers.renderGroupsSizes.marker++] = (unsigned)rg.quadsData.size();
			else
				mDebugNumbers.notAffectedByLightRenderGroupsSizes.data[mDebugNumbers.notAffectedByLightRenderGroupsSizes.marker++] = (unsigned)rg.quadsData.size();
		}

		// set up shader
		if(key.shader != mCurrentlyBoundQuadShader) 
		{
			key.shader->bind();
			mCurrentlyBoundQuadShader = key.shader;

			int textures[32];
			for(int i = 0; i < 32; ++i)
				textures[i] = i;
			key.shader->setUniformIntArray("textures", 32, textures);
		}
		key.shader->setUniformFloat("z", key.z);
		key.shader->setUniformBool("isGameWorldProjection", key.projectionType == ProjectionType::gameWorld);

		// sort quads by texture slot ref
		std::sort(rg.quadsData.begin(), rg.quadsData.end(), [](const QuadData& a, const QuadData& b) {
			return a.textureSlotRef < b.textureSlotRef;
		});

		// draw render group
		for(unsigned i = 0; i < static_cast<unsigned>(rg.quadsData.size()); ++i)
		{
			if(i == static_cast<unsigned>(rg.quadsData.size()) - 1)
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
		textures[i]->bind(static_cast<unsigned>(i));
}

void QuadRenderer::drawCall(unsigned nrOfInstances, std::vector<QuadData>& quadsData)
{
	GLCheck( glBindBuffer(GL_ARRAY_BUFFER, mQuadsDataVBO) );
	GLCheck( glBufferData(GL_ARRAY_BUFFER, nrOfInstances * sizeof(QuadData), quadsData.data(), GL_STATIC_DRAW) );

	GLCheck( glBindVertexArray(mVAO) );
	GLCheck( glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, nrOfInstances) );

	if(mIsDebugCountingActive)
		++mDebugNumbers.drawCalls;
}

}
