#include "quadRenderer.hpp"
#include "Renderer/API/texture.hpp"
#include "Renderer/API/openglErrors.hpp"
#include "Renderer/Shaders/embeddedShaders.hpp"
#include "Utilities/cast.hpp"
#include "Utilities/profiling.hpp"
#include "Utilities/math.hpp"
#include "Logs/logs.hpp"
#include <GL/glew.h>
#include <algorithm>
#include <cstdlib>
#include <vector>

namespace ph {

static constexpr unsigned deleteVBOsDelay = 100;

struct RenderGroupsHashMap
{
	unsigned capacity;
	unsigned size;
	unsigned* indices = nullptr;
	RenderGroupKey* keys = nullptr;
	QuadRenderGroup* renderGroups = nullptr;
	bool needsToBeSorted = false;
};

struct GroundChunk
{
	FloatRect textureRect;
	sf::Vector2f pos;
	unsigned texture;
	float z;
};

struct Chunk
{
	FloatRect bounds;
	unsigned rendererID;
	unsigned texture;
	unsigned quadsCount;
	float z;
};

struct ChunksData
{
	std::vector<unsigned> cachedChunksRendererIDs;
	std::vector<Chunk> thisFrameChunks;
	unsigned dummyVAO;
	unsigned framesToDeleteChunkVBOs = deleteVBOsDelay;
	bool allChunkVBOsWereJustDeleted = true;
};

static std::vector<GroundChunk> groundChunks;
static ChunksData chunks;

static RenderGroupsHashMap renderGroupsHashMap;
static RenderGroupsHashMap notAffectedByLightRenderGroupsHashMap;

static QuadRendererDebugNumbers debugNumbers;

static Shader defaultQuadShader;
static Shader groundChunkShader;
static Shader chunkShader;

QuadRendererDebugNumbers getQuadRendererDebugNumbers()
{
	return debugNumbers;
}

void resetQuadRendererDebugNumbers()
{
	debugNumbers.renderGroupsSizes = {}; 
	debugNumbers.notAffectedByLightRenderGroupsSizes = {}; 
	debugNumbers.drawCalls = 0;
	debugNumbers.chunks = 0;
	debugNumbers.cachedChunks = 0;
	debugNumbers.drawnSprites = 0;
	debugNumbers.drawnTextures = 0;
}

void setQuadRendererDebug(bool flag)
{
	defaultQuadShader.bind();
	defaultQuadShader.setUniformBool("debugVisualization", flag);

	chunkShader.bind();
	chunkShader.setUniformBool("debugVisualization", flag);

	groundChunkShader.bind();
	groundChunkShader.setUniformBool("debugVisualization", flag);
}

static unsigned bumpToNext4000(unsigned size)
{
	unsigned temp = size;
	while(temp > 4000)
		temp -= 4000;
	return size + 4000 - temp;	
}

static bool operator==(const RenderGroupKey& lhs, const RenderGroupKey& rhs)
{
	return lhs.shader == rhs.shader && lhs.z == rhs.z;
}

static QuadRenderGroup* insertIfDoesNotExitstAndGetRenderGroup(RenderGroupsHashMap* hashMap, RenderGroupKey key, unsigned quadDataCount)
{
	// return render group of matching key if exists
	for(unsigned i = 0; i < hashMap->size; ++i)
		if(key == *(hashMap->keys + i))
			return hashMap->renderGroups + i;

	// bump debug numbers
	if(hashMap == &renderGroupsHashMap)
	{
		debugNumbers.renderGroups = hashMap->size;
		debugNumbers.renderGroupsZ.data[debugNumbers.renderGroupsZ.marker++] = (unsigned)(key.z * 255);
	}
	else if(hashMap == &notAffectedByLightRenderGroupsHashMap)
	{
		debugNumbers.renderGroupsNotAffectedByLight = hashMap->size; 
		debugNumbers.notAffectedByLightRenderGroupsZ.data[debugNumbers.notAffectedByLightRenderGroupsZ.marker++] = (unsigned)(key.z * 255);
	}

	// reallocate if there is no more space
	if(hashMap->capacity == hashMap->size)
	{
		hashMap->capacity *= 2;

		void* newIndices = realloc(hashMap->indices, hashMap->capacity * sizeof(unsigned));
		PH_ASSERT_CRITICAL(newIndices, "quad render group hash map indices realloc failed!");
		hashMap->indices = (unsigned*)newIndices;

		void* newKeys = realloc(hashMap->keys, hashMap->capacity * sizeof(RenderGroupKey));
		PH_ASSERT_CRITICAL(newKeys, "quad render group hash map keys realloc failed!");
		hashMap->keys = (RenderGroupKey*)newKeys;

		void* newRenderGroups = realloc(hashMap->renderGroups, hashMap->capacity * sizeof(QuadRenderGroup));
		PH_ASSERT_CRITICAL(newRenderGroups, "quad render group hash map render groups realloc failed!");
		hashMap->renderGroups = (QuadRenderGroup*)newRenderGroups;

		debugNumbers.allocations += 3;
	}

	// insert new index
	hashMap->indices[hashMap->size] = hashMap->size;
	hashMap->needsToBeSorted = true;

	// insert new key
	hashMap->keys[hashMap->size] = key;

	// insert new group
	quadDataCount = bumpToNext4000(quadDataCount);
	size_t arenaSize = sizeof(QuadData) * quadDataCount; 
	QuadRenderGroup& qrg = hashMap->renderGroups[hashMap->size];
	qrg.quadsDataArenaSize = (unsigned)arenaSize; 
	qrg.texturesSize = 0;
	qrg.texturesCapacity = 32;
	qrg.textures = (unsigned*)malloc(sizeof(unsigned) * 32);
	qrg.quadsDataSize = 0; 
	qrg.quadsDataCapacity = quadDataCount; 
	qrg.quadsData = (QuadData*)malloc(arenaSize);
	debugNumbers.allocations += 2;

	++hashMap->size;

	return &qrg; 
}

static auto getTextureSlotToWhichThisTextureIsBound(const Texture* texture, QuadRenderGroup* rg) -> std::optional<float>
{
	unsigned* ptr = rg->textures;
	for(size_t i = 0; i < rg->texturesSize; ++i, ++ptr)
		if(*ptr == texture->getID())
			return static_cast<float>(i);
	return std::nullopt;
}

static void insertQuadDataToQuadRenderGroup(QuadData* quadData, unsigned count, QuadRenderGroup* quadRenderGroup)
{
	bool thereIsNoPlaceForNewQuadData = quadRenderGroup->quadsDataSize + count > quadRenderGroup->quadsDataCapacity;
	if(thereIsNoPlaceForNewQuadData)
	{
		// reallocate quad render group arena
		size_t oldArenaSize = (size_t)quadRenderGroup->quadsDataArenaSize; 
		void* oldArena = (void*)quadRenderGroup->quadsData; 
		unsigned newQuadsDataCapacity = quadRenderGroup->quadsDataCapacity + count * 2; 
		size_t newArenaSize = newQuadsDataCapacity * sizeof(QuadData); 
		quadRenderGroup->quadsDataArenaSize = (unsigned)newArenaSize;
		quadRenderGroup->quadsDataCapacity = newQuadsDataCapacity; 
		quadRenderGroup->quadsData = (QuadData*)malloc(newArenaSize);
		memcpy(quadRenderGroup->quadsData, oldArena, oldArenaSize);
		free(oldArena);

		++debugNumbers.allocations;
	}

	void* writePtr = quadRenderGroup->quadsData + (quadRenderGroup->quadsDataSize);
	memcpy(writePtr, quadData, sizeof(QuadData) * count);
	quadRenderGroup->quadsDataSize += count;
}

static void initRenderGroupsHashMap(RenderGroupsHashMap& hashMap)
{
	constexpr size_t initialGroupsCapacity = 100;
	hashMap.capacity = initialGroupsCapacity;
	hashMap.size = 0;
	hashMap.indices = (unsigned*)malloc(sizeof(unsigned) * initialGroupsCapacity);
	hashMap.keys = (RenderGroupKey*)malloc(sizeof(RenderGroupKey) * initialGroupsCapacity);
	hashMap.renderGroups = (QuadRenderGroup*)malloc(sizeof(QuadRenderGroup) * initialGroupsCapacity);
	debugNumbers.allocations += 3;
}

void QuadRenderer::init()
{
	// allocate memory
	static bool shouldInitializeRenderGroups = true;
	if(shouldInitializeRenderGroups)
	{
		initRenderGroupsHashMap(renderGroupsHashMap);
		initRenderGroupsHashMap(notAffectedByLightRenderGroupsHashMap);
		shouldInitializeRenderGroups = false;
	}
	groundChunks.reserve(20);
	chunks.thisFrameChunks.reserve(50);

	// init shaders
	defaultQuadShader.init(shader::quadSrc());
	defaultQuadShader.initUniformBlock("SharedData", 0);

	groundChunkShader.init(shader::groundChunkSrc());
	groundChunkShader.initUniformBlock("SharedData", 0);

	chunkShader.init(shader::chunkSrc());
	chunkShader.initUniformBlock("SharedData", 0);

	// create vao and ibo for quads from hash map 
	GLCheck( glGenVertexArrays(1, &mVAO) );
	GLCheck( glBindVertexArray(mVAO) );

	unsigned quadIndices[] = {0, 1, 3, 1, 2, 3};
	GLCheck( glGenBuffers(1, &mQuadIBO) );
	GLCheck( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mQuadIBO) );
	GLCheck( glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW) ); 

	GLCheck( glGenBuffers(1, &mQuadsDataVBO) );
	GLCheck( glBindBuffer(GL_ARRAY_BUFFER, mQuadsDataVBO) );

	GLCheck( glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(QuadData), (void*)offsetof(QuadData, color)) );
	GLCheck( glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(QuadData), (void*)offsetof(QuadData, textureRect)) );
	GLCheck( glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(QuadData), (void*)offsetof(QuadData, position)) );
	GLCheck( glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(QuadData), (void*)offsetof(QuadData, size)) );
	GLCheck( glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(QuadData), (void*)offsetof(QuadData, rotationOrigin)) );
	GLCheck( glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(QuadData), (void*)offsetof(QuadData, rotation)) );
	GLCheck( glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, sizeof(QuadData), (void*)offsetof(QuadData, textureSlotRef)) );

	for(int i = 0; i < 7; ++i) {
		GLCheck( glEnableVertexAttribArray(i) );
	}
	for(int i = 0; i < 7; ++i) {
		GLCheck( glVertexAttribDivisor(i, 1) );
	}

	// create vao for chunks
	glGenVertexArrays(1, &chunks.dummyVAO);

	// create white texture
	mWhiteTexture = new Texture;
	unsigned white = 0xffffffff;
	mWhiteTexture->setData(&white, sizeof(unsigned), sf::Vector2i(1, 1));
}

void QuadRenderer::shutDown()
{
	delete mWhiteTexture;
	defaultQuadShader.remove();
	groundChunkShader.remove();
	chunkShader.remove();
	GLCheck( glDeleteBuffers(1, &mQuadIBO) );
	GLCheck( glDeleteBuffers(1, &mQuadsDataVBO) );
	GLCheck( glDeleteVertexArrays(1, &mVAO) );
	glDeleteVertexArrays(1, &chunks.dummyVAO);
}

void QuadRenderer::submitGroundChunk(sf::Vector2f pos, const Texture& texture, const FloatRect& textureRect, float z)
{
	groundChunks.push_back(GroundChunk{textureRect, pos, texture.getID(), z});
}

void QuadRenderer::submitChunk(std::vector<ChunkQuadData>& quadsData, const Texture& texture, const FloatRect& bounds,
                               float z, unsigned* rendererID)
{
	// check is the vbo of this chunk in VRam
	bool vboOfThisChunkIsInVRam = false;
	if(!chunks.allChunkVBOsWereJustDeleted || *rendererID != 0)
	{
		for(unsigned vbo : chunks.cachedChunksRendererIDs)
		{
			if(*rendererID == vbo)
			{
				vboOfThisChunkIsInVRam = true;
				break;
			}
		}
	}

	if(!vboOfThisChunkIsInVRam)
	{
		// create the new vbo
		glBindVertexArray(0);
		glGenBuffers(1, rendererID);
		glBindBuffer(GL_ARRAY_BUFFER, *rendererID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(ChunkQuadData) * quadsData.size(), quadsData.data(), GL_STATIC_DRAW);
		chunks.cachedChunksRendererIDs.emplace_back(*rendererID);
	}

	chunks.thisFrameChunks.emplace_back(Chunk{bounds, *rendererID, texture.getID(), (unsigned)quadsData.size(), z});
}

void QuadRenderer::submitBunchOfQuadsWithTheSameTexture(std::vector<QuadData>& quadsData, Texture* texture,
                                                        const Shader* shader, float z, ProjectionType projectionType)
{
	if(!shader)
		shader = &defaultQuadShader;

	// insert if does not exitst and get render group
	bool isAffectedByLight = true; // TODO
	RenderGroupsHashMap& hashMap = isAffectedByLight ? renderGroupsHashMap : notAffectedByLightRenderGroupsHashMap;
	QuadRenderGroup* renderGroup = insertIfDoesNotExitstAndGetRenderGroup(&hashMap, {shader, z, projectionType}, (unsigned)quadsData.size());

	if(!texture)
		texture = mWhiteTexture;
	auto textureSlotOfThisTexture = getTextureSlotToWhichThisTextureIsBound(texture, renderGroup);
	if(textureSlotOfThisTexture) 
	{
		for(auto& quad : quadsData)
			quad.textureSlotRef = *textureSlotOfThisTexture;
	}
	else 
	{
		for(auto& quad : quadsData)
			quad.textureSlotRef = (float)renderGroup->texturesSize;
		renderGroup->textures[renderGroup->texturesSize] = texture->getID();
		++renderGroup->texturesSize;
	}

	insertQuadDataToQuadRenderGroup(quadsData.data(), (unsigned)quadsData.size(), renderGroup);
}

void QuadRenderer::submitQuad(Texture* texture, const IntRect* textureRect, const sf::Color* color, const Shader* shader,
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
		shader = &defaultQuadShader;

	// find or add draw call group
	auto& hashMap = isAffectedByLight ? renderGroupsHashMap : notAffectedByLightRenderGroupsHashMap;
	QuadRenderGroup* renderGroup = insertIfDoesNotExitstAndGetRenderGroup(&hashMap, {shader, z, projectionType}, 1);

	// compute final texture rect
	FloatRect finalTextureRect;
	if(textureRect)
	{
		auto ts = static_cast<sf::Vector2f>(texture->getSize());
		finalTextureRect = FloatRect(
			textureRect->left / ts.x, (ts.y - textureRect->top - textureRect->height) / ts.y,
			textureRect->width / ts.x, textureRect->height / ts.y
		);
	}
	else
	{
		finalTextureRect = FloatRect(0.f, 0.f, 1.f, 1.f);	
	}

	// submit data
	QuadData quadData;
	
	quadData.color = color ? Cast::toNormalizedColorVector4f(*color) : Cast::toNormalizedColorVector4f(sf::Color::White);
	quadData.textureRect = finalTextureRect; 
	quadData.position = position;
	quadData.size = size;
	quadData.rotationOrigin = rotationOrigin;
	quadData.rotation = Math::degreesToRadians(rotation);
	
	if(!texture)
		texture = mWhiteTexture;
	if(auto textureSlotOfThisTexture = getTextureSlotToWhichThisTextureIsBound(texture, renderGroup))
	{
		quadData.textureSlotRef = *textureSlotOfThisTexture;
	}
	else 
	{
		unsigned textureSlotID = renderGroup->texturesSize;
		quadData.textureSlotRef = (float)textureSlotID;
		*(renderGroup->textures + textureSlotID) = texture->getID();
		++renderGroup->texturesSize;
	}

	insertQuadDataToQuadRenderGroup(&quadData, 1, renderGroup);
}

void QuadRenderer::flush(bool affectedByLight)
{
	PH_PROFILE_FUNCTION();

	const Shader* currentlyBoundShader = nullptr;
	auto& hashMap = affectedByLight ? renderGroupsHashMap : notAffectedByLightRenderGroupsHashMap;

	// sort hash map indices
	if(hashMap.needsToBeSorted)
	{
		PH_PROFILE_SCOPE("sorting indices");
		hashMap.needsToBeSorted = false;
		for(unsigned i = 0; i < hashMap.size - 1; ++i)
		{
			for(unsigned j = 0; j < hashMap.size - 1; ++j)
			{
				unsigned index1 = hashMap.indices[j];
				unsigned index2 = hashMap.indices[j + 1];
				if(hashMap.keys[index1].z < hashMap.keys[index2].z)
				{
					unsigned temp = hashMap.indices[j];
					hashMap.indices[j] = hashMap.indices[j + 1];
					hashMap.indices[j + 1] = temp; 
				}
			}
		}
		
		debugNumbers.renderGroupsIndices.marker = 0;
		auto& debugIndices = affectedByLight ? debugNumbers.renderGroupsIndices : debugNumbers.notAffectedByLightRenderGroupsIndices;
		for(unsigned i = 0; i < hashMap.size; ++i)
		{
			debugNumbers.renderGroupsIndices.data[i] = hashMap.indices[i];
			++debugNumbers.renderGroupsIndices.marker;
		}
	}

	if(affectedByLight)
	{
		// sort ground chunks
		std::sort(groundChunks.begin(), groundChunks.end(), []
		(const GroundChunk& lhs, const GroundChunk& rhs)
		{
			return lhs.z > rhs.z;
		});

		// sort chunks
		std::sort(chunks.thisFrameChunks.begin(), chunks.thisFrameChunks.end(), []
		(const Chunk& lhs, const Chunk& rhs)
		{
			return lhs.z > rhs.z;
		});

		// debug info
		debugNumbers.framesToDeleteChunkVBOs = chunks.framesToDeleteChunkVBOs; 
	}

	for(unsigned hashMapIndex = 0, chunkIndex = 0, groundChunkIndex = 0;
	    hashMapIndex + chunkIndex + groundChunkIndex <
		hashMap.size + chunks.thisFrameChunks.size() + groundChunks.size();)
	{
		enum DrawFrom{HashMap, Chunks, GroundChunks};
		DrawFrom drawFrom = HashMap;

		float hashMapZ = hashMapIndex < hashMap.size ? hashMap.keys[hashMapIndex].z : -1.f;
		float chunkZ = chunkIndex < chunks.thisFrameChunks.size() ? chunks.thisFrameChunks[chunkIndex].z : -1.f;
		float groundChunkZ = groundChunkIndex < groundChunks.size() ? groundChunks[groundChunkIndex].z : -1.f;

		if((groundChunkZ != -1.f) &&
		   (groundChunkZ >= chunkZ && groundChunkZ >= hashMapZ)) 
		{
			drawFrom = GroundChunks;
		}
		else if((chunkZ != -1.f) &&
		        (chunkZ >= groundChunkZ && chunkZ >= hashMapZ)) 
		{
			drawFrom = Chunks;
		}
		else
		{
			drawFrom = HashMap;
		}

		switch(drawFrom)
		{
			case HashMap: 
			{
				PH_PROFILE_SCOPE("draw hash map", 0);

				unsigned renderGroupIndex = hashMap.indices[hashMapIndex];
				auto& key = hashMap.keys[renderGroupIndex];
				auto& rg = hashMap.renderGroups[renderGroupIndex];

				// update debug info
				if(mIsDebugCountingActive) 
				{
					debugNumbers.drawnSprites += rg.quadsDataSize;
					debugNumbers.drawnTextures += rg.texturesSize;
					if(affectedByLight)
						debugNumbers.renderGroupsSizes.data[debugNumbers.renderGroupsSizes.marker++] = rg.quadsDataSize;
					else
						debugNumbers.notAffectedByLightRenderGroupsSizes.data[debugNumbers.notAffectedByLightRenderGroupsSizes.marker++] = rg.quadsDataSize;
				}

				// set up shader
				if(key.shader != currentlyBoundShader) 
				{
					key.shader->bind();
					currentlyBoundShader = key.shader;

					int textures[32];
					for(int i = 0; i < 32; ++i)
						textures[i] = i;
					key.shader->setUniformIntArray("textures", 32, textures);
				}
				key.shader->setUniformFloat("z", key.z);
				key.shader->setUniformBool("isGameWorldProjection", key.projectionType == ProjectionType::gameWorld);

				// TODO: sort quads by texture slot ref if there are more then 32 
				// std::sort(rg.quadsData.begin(), rg.quadsData.end(), [](const QuadData& a, const QuadData& b) { return a.textureSlotRef < b.textureSlotRef; });

				// draw render group
				unsigned quadsDataSize = rg.quadsDataSize;
				unsigned texturesSize = rg.texturesSize;
				QuadData* quadsData = rg.quadsData;
				unsigned* textures = rg.textures;

				auto bindTexturesForNextDrawCall = [textures, texturesSize]
				{
					for(unsigned textureSlot = 0; textureSlot < (texturesSize > 32 ? 32 : texturesSize); ++textureSlot)
					{
						glActiveTexture(GL_TEXTURE0 + textureSlot);
						glBindTexture(GL_TEXTURE_2D, *(textures + textureSlot));
					}
				};

				auto drawCall = [this, quadsData](size_t nrOfInstances)
				{
					char log[50];
					sprintf(log, "draw call instances: %zu", nrOfInstances);
					PH_PROFILE_SCOPE(log, 0);

					GLCheck( glBindVertexArray(mVAO) );
					GLCheck( glBindBuffer(GL_ARRAY_BUFFER, mQuadsDataVBO) );
					GLCheck( glBufferData(GL_ARRAY_BUFFER, nrOfInstances * sizeof(QuadData), quadsData, GL_STATIC_DRAW) );

					GLCheck( glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, (GLsizei)nrOfInstances) );

					if(mIsDebugCountingActive)
						++debugNumbers.drawCalls;
				};

				for(size_t i = 0; i < rg.quadsDataSize; ++i)
				{
					if(i == rg.quadsDataSize - 1)
					{
						bindTexturesForNextDrawCall();
						drawCall(i + 1);
						break;
					}
					else if(rg.quadsData[i + 1].textureSlotRef == 32)
					{
						bindTexturesForNextDrawCall();
						drawCall(i + 1);

						quadsData += i;

						QuadData* ptr = quadsData;
						for(unsigned i = 0; i < quadsDataSize; ++i)
						{
							ptr->textureSlotRef -= 32;
							++ptr;
						}

						if(texturesSize > 32)
						{
							textures += 32;
							texturesSize -= 32;
						}
						else
						{
							texturesSize = 0;
						}

						i = 0;
					}
				}

				rg.quadsDataSize = 0;
				rg.texturesSize = 0;

				++hashMapIndex;
			} break;

			case GroundChunks:
			{
				PH_PROFILE_SCOPE("draw ground chunk", 0);

				auto& gc = groundChunks[groundChunkIndex];

				glBindVertexArray(0);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, gc.texture);

				if(currentlyBoundShader != &groundChunkShader)
				{
					currentlyBoundShader = &groundChunkShader;
					groundChunkShader.bind();
				}
				groundChunkShader.setUniformVector2("chunkPos", gc.pos);
				groundChunkShader.setUniformFloat("z", gc.z);
				groundChunkShader.setUniformVector2("uvTopLeft", gc.textureRect.getTopLeft());
				groundChunkShader.setUniformVector2("uvTopRight", gc.textureRect.getTopRight());
				groundChunkShader.setUniformVector2("uvBottomLeft", gc.textureRect.getBottomLeft());
				groundChunkShader.setUniformVector2("uvBottomRight", gc.textureRect.getBottomRight());

				glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, 144);

				++groundChunkIndex;
				
				if(mIsDebugCountingActive)
					++debugNumbers.drawCalls;
			} break;

			case Chunks:
			{
				PH_PROFILE_SCOPE("draw chunk", 0);

				auto& chunk = chunks.thisFrameChunks[chunkIndex];

				GLCheck( glActiveTexture(GL_TEXTURE0) );
				GLCheck( glBindTexture(GL_TEXTURE_2D, chunk.texture) );

				if(currentlyBoundShader != &chunkShader)
				{
					currentlyBoundShader = &chunkShader;
					chunkShader.bind();
				}
				chunkShader.setUniformFloat("z", chunk.z);

				GLCheck( glBindVertexArray(chunks.dummyVAO) );
				GLCheck( glBindBuffer(GL_ARRAY_BUFFER, chunk.rendererID) );

				GLCheck( glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(ChunkQuadData), (void*)offsetof(ChunkQuadData, textureRect)) );
				GLCheck( glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(ChunkQuadData), (void*)offsetof(ChunkQuadData, position)) );
				GLCheck( glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ChunkQuadData), (void*)offsetof(ChunkQuadData, size)) );
				GLCheck( glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(ChunkQuadData), (void*)offsetof(ChunkQuadData, rotation)) );

				for(int i = 0; i < 4; ++i)
					glEnableVertexAttribArray(i);

				for(int i = 0; i < 4; ++i)
					glVertexAttribDivisor(i, 1);

				GLCheck( glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, chunk.quadsCount) );

				++chunkIndex;

				if(mIsDebugCountingActive)
				{
					++debugNumbers.chunks; 
					++debugNumbers.drawCalls;
				}
			} break;
		}
	}
		
	if(affectedByLight)
	{
		if(--chunks.framesToDeleteChunkVBOs == 0)
		{
			// clear cached chunks
			chunks.framesToDeleteChunkVBOs = deleteVBOsDelay;
			for(unsigned id : chunks.cachedChunksRendererIDs)
			{
				glDeleteBuffers(1, &id); 
			}
			chunks.cachedChunksRendererIDs.clear();
			chunks.allChunkVBOsWereJustDeleted = true;
		}
		else
		{
			chunks.allChunkVBOsWereJustDeleted = false;
		}

		chunks.thisFrameChunks.clear();
		groundChunks.clear();

		debugNumbers.cachedChunks = (unsigned)chunks.cachedChunksRendererIDs.size();
	}
}

}
