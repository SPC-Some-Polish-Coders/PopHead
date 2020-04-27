#include "quadRenderer.hpp"
#include "Renderer/API/texture.hpp"
#include "Renderer/API/openglErrors.hpp"
#include "Renderer/Shaders/embeddedShaders.hpp"
#include "Utilities/cast.hpp"
#include "Utilities/profiling.hpp"
#include "Utilities/math.hpp"
#include "Logs/logs.hpp"
#include <GL/glew.h>
#include <imgui.h>
#include <algorithm>
#include <cstdlib>
#include <vector>

namespace ph::QuadRenderer {

static constexpr unsigned deleteVBOsDelay = 500;

struct DebugNumbers
{
	std::vector<unsigned> renderGroupsSizes; 
	std::vector<unsigned> renderGroupsZ; 
	std::vector<unsigned> renderGroupsIndices; 
	std::vector<unsigned> notAffectedByLightRenderGroupsSizes; 
	std::vector<unsigned> notAffectedByLightRenderGroupsZ; 
	std::vector<unsigned> notAffectedByLightRenderGroupsIndices; 
	unsigned allocations = 0;
	unsigned chunks = 0;
	unsigned framesToDeleteChunkVBOs = 0;
	unsigned renderGroups = 0;
	unsigned renderGroupsNotAffectedByLight = 0;
	unsigned drawCalls = 0;
	unsigned drawnSprites = 0;
	unsigned drawnTextures = 0;
};

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
	float z;
};

struct Chunk
{
	FloatRect bounds;
	unsigned vbo;
	unsigned quadsCount;
	float z;
};

struct RegisteredChunk
{
	FloatRect bounds;
	unsigned id;
	unsigned vbo;
};

struct ChunksData
{
	std::vector<RegisteredChunk> registerChunks;
	std::vector<Chunk> thisFrameChunks;
	unsigned nextRegisteredChunkID = 0;
	unsigned dummyVAO;
	unsigned framesToDeleteChunkVBOs = deleteVBOsDelay;
};

static ChunksData chunks;

static std::vector<GroundChunk> groundChunks;
static unsigned groundChunkVAO; 

static unsigned chunksTexture; // for chunks and ground chunks

static RenderGroupsHashMap renderGroupsHashMap;
static RenderGroupsHashMap notAffectedByLightRenderGroupsHashMap;

static DebugNumbers debugNumbers;

static Shader defaultQuadShader;
static Shader groundChunkShader;
static Shader chunkShader;

static const FloatRect* screenBounds; 
static Texture* whiteTexture;
static unsigned quadIBO;
static unsigned quadsDataVBO;
static unsigned quadVao;
static bool isDebugCountingActive = false;

void setChunksTexture(unsigned texture)
{
	chunksTexture = texture;
}

DebugNumbers getDebugNumbers()
{
	return debugNumbers;
}

void setScreenBoundsPtr(const FloatRect* bounds)
{
	screenBounds = bounds;		
}

void resetDebugNumbers()
{
	debugNumbers.renderGroupsSizes.clear(); 
	debugNumbers.notAffectedByLightRenderGroupsSizes.clear(); 
	debugNumbers.drawCalls = 0;
	debugNumbers.chunks = 0;
	debugNumbers.drawnSprites = 0;
	debugNumbers.drawnTextures = 0;
}

void setDebug(bool flag)
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
		debugNumbers.renderGroupsZ.emplace_back((unsigned)(key.z * 255));
	}
	else if(hashMap == &notAffectedByLightRenderGroupsHashMap)
	{
		debugNumbers.renderGroupsNotAffectedByLight = hashMap->size; 
		debugNumbers.notAffectedByLightRenderGroupsZ.emplace_back((unsigned)(key.z * 255));
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

void init()
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

	debugNumbers.renderGroupsIndices.resize(100);
	debugNumbers.notAffectedByLightRenderGroupsIndices.resize(100);

	// init shaders
	defaultQuadShader.init(shader::quadSrc());
	defaultQuadShader.initUniformBlock("SharedData", 0);

	groundChunkShader.init(shader::groundChunkSrc());
	groundChunkShader.initUniformBlock("SharedData", 0);

	chunkShader.init(shader::chunkSrc());
	chunkShader.initUniformBlock("SharedData", 0);

	// create vao and ibo for quads from hash map 
	GLCheck( glGenVertexArrays(1, &quadVao) );
	GLCheck( glBindVertexArray(quadVao) );

	GLCheck( glGenBuffers(1, &quadsDataVBO) );
	GLCheck( glBindBuffer(GL_ARRAY_BUFFER, quadsDataVBO) );

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

	// create dummy vao for chunks
	glGenVertexArrays(1, &chunks.dummyVAO);

	// create vao and vbo for ground chunks
	glGenVertexArrays(1, &groundChunkVAO);
	glBindVertexArray(groundChunkVAO);

	float groundChunkVertices[] = {
		0.f, 0.f,
		16.f, 0.f,
		0.f, 16.f,
		16.f, 16.f
	};
	unsigned groundChunkVBO;
	glGenBuffers(1, &groundChunkVBO);
	glBindBuffer(GL_ARRAY_BUFFER, groundChunkVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(groundChunkVertices), groundChunkVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

	// create white texture
	whiteTexture = new Texture;
	unsigned white = 0xffffffff;
	whiteTexture->setData(&white, sizeof(unsigned), sf::Vector2i(1, 1));
}

void shutDown()
{
	delete whiteTexture;
	defaultQuadShader.remove();
	groundChunkShader.remove();
	chunkShader.remove();
	GLCheck( glDeleteBuffers(1, &quadIBO) );
	GLCheck( glDeleteBuffers(1, &quadsDataVBO) );
	GLCheck( glDeleteVertexArrays(1, &quadVao) );
	glDeleteVertexArrays(1, &chunks.dummyVAO);
}

void submitGroundChunk(sf::Vector2f pos, const FloatRect& textureRect, float z)
{
	groundChunks.push_back(GroundChunk{textureRect, pos, z});
}

unsigned registerNewChunk(const FloatRect& bounds)
{
	chunks.registerChunks.emplace_back(RegisteredChunk{bounds, chunks.nextRegisteredChunkID, 0});
	++chunks.nextRegisteredChunkID;
	return chunks.nextRegisteredChunkID - 1;
}

void submitChunk(std::vector<ChunkQuadData>& quadsData, const FloatRect& bounds,
                 float z, unsigned* id)
{
	for(auto& cached : chunks.registerChunks)
	{
		if(*id == cached.id) 
		{
			if(cached.vbo)
			{
				chunks.thisFrameChunks.emplace_back(Chunk{bounds, cached.vbo, (unsigned)quadsData.size(), z});
			}
			else
			{
				// create the new vbo
				glBindVertexArray(0);
				glGenBuffers(1, &cached.vbo);
				glBindBuffer(GL_ARRAY_BUFFER, cached.vbo);
				glBufferData(GL_ARRAY_BUFFER, sizeof(ChunkQuadData) * quadsData.size(), quadsData.data(), GL_STATIC_DRAW);
			}
			return;
		}
	}
	PH_UNEXPECTED_SITUATION("chunk of this ID isn't cached in quad renderer");
}

void submitBunchOfQuadsWithTheSameTexture(std::vector<QuadData>& quadsData, Texture* texture,
                                          const Shader* shader, float z, ProjectionType projectionType)
{
	if(!shader)
		shader = &defaultQuadShader;

	// insert if does not exitst and get render group
	bool isAffectedByLight = true; // TODO
	RenderGroupsHashMap& hashMap = isAffectedByLight ? renderGroupsHashMap : notAffectedByLightRenderGroupsHashMap;
	QuadRenderGroup* renderGroup = insertIfDoesNotExitstAndGetRenderGroup(&hashMap, {shader, z, projectionType}, (unsigned)quadsData.size());

	if(!texture)
		texture = whiteTexture;
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

void submitQuad(Texture* texture, const IntRect* textureRect, const sf::Color* color, const Shader* shader,
                sf::Vector2f position, sf::Vector2f size, float z, float rotation, sf::Vector2f rotationOrigin,
                ProjectionType projectionType, bool isAffectedByLight)
{
	// culling
	FloatRect bounds = projectionType == ProjectionType::gameWorld ? *screenBounds : FloatRect(0.f, 0.f, 1920.f, 1080.f);
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
		texture = whiteTexture;
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

void flush(bool affectedByLight)
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
		
		debugNumbers.renderGroupsIndices.clear();
		auto& debugIndices = affectedByLight ? debugNumbers.renderGroupsIndices : debugNumbers.notAffectedByLightRenderGroupsIndices;

		/* TODO
		for(unsigned i = 0; i < hashMap.size; ++i)
		{
			debugNumbers.renderGroupsIndices[i] = hashMap.indices[i];
		}
		*/
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

		for(unsigned chunkIndex = 0, groundChunkIndex = 0;
			chunkIndex + groundChunkIndex < chunks.thisFrameChunks.size() + groundChunks.size();)
		{
			float chunkZ = chunkIndex < chunks.thisFrameChunks.size() ? chunks.thisFrameChunks[chunkIndex].z : -1.f;
			float groundChunkZ = groundChunkIndex < groundChunks.size() ? groundChunks[groundChunkIndex].z : -1.f;

			if((groundChunkZ != -1.f) && (groundChunkZ >= chunkZ)) 
			{
				// draw ground chunk
				PH_PROFILE_SCOPE("draw ground chunk");

				auto& gc = groundChunks[groundChunkIndex];

				GLCheck( glBindVertexArray(groundChunkVAO) );
				GLCheck( glActiveTexture(GL_TEXTURE0) );
				GLCheck( glBindTexture(GL_TEXTURE_2D, chunksTexture) );

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

				GLCheck( glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, 144) );

				++groundChunkIndex;
				
				if(isDebugCountingActive)
					++debugNumbers.drawCalls;
			}
			else
			{
				// draw chunk
				PH_PROFILE_SCOPE("draw chunk");

				auto& chunk = chunks.thisFrameChunks[chunkIndex];

				GLCheck( glActiveTexture(GL_TEXTURE0) );
				GLCheck( glBindTexture(GL_TEXTURE_2D, chunksTexture) );

				if(currentlyBoundShader != &chunkShader)
				{
					currentlyBoundShader = &chunkShader;
					chunkShader.bind();
				}
				chunkShader.setUniformFloat("z", chunk.z);

				GLCheck( glBindVertexArray(chunks.dummyVAO) );
				GLCheck( glBindBuffer(GL_ARRAY_BUFFER, chunk.vbo) );

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

				if(isDebugCountingActive)
				{
					++debugNumbers.chunks; 
					++debugNumbers.drawCalls;
				}
			}
		}

		if(--chunks.framesToDeleteChunkVBOs == 0)
		{
			// delete vbos of chunks outside camera 

			chunks.framesToDeleteChunkVBOs = deleteVBOsDelay;

			for(auto& cached : chunks.registerChunks)
			{
				if(cached.vbo && !screenBounds->doPositiveRectsIntersect(cached.bounds))
				{
					glDeleteBuffers(1, &cached.vbo);
					cached.vbo = 0;
				}
			}
		}

		chunks.thisFrameChunks.clear();
		groundChunks.clear();
	}

	// NOTE: We assume that quads it hash map are always at top of ground chunks and chunks

	// draw from hash map
	for(unsigned pairIndex = 0; pairIndex < hashMap.size; ++pairIndex)
	{
		PH_PROFILE_SCOPE("draw hash map pair");
		unsigned renderGroupIndex = hashMap.indices[pairIndex];
		auto& key = hashMap.keys[renderGroupIndex];
		auto& rg = hashMap.renderGroups[renderGroupIndex];

		// update debug info
		if(isDebugCountingActive) 
		{
			debugNumbers.drawnSprites += rg.quadsDataSize;
			debugNumbers.drawnTextures += rg.texturesSize;
			if(affectedByLight)
				debugNumbers.renderGroupsSizes.emplace_back(rg.quadsDataSize);
			else
				debugNumbers.notAffectedByLightRenderGroupsSizes.emplace_back(rg.quadsDataSize);
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

		auto drawCall = [quadsData](size_t nrOfInstances)
		{
			char log[50];
			sprintf(log, "draw call instances: %zu", nrOfInstances);
			PH_PROFILE_SCOPE(log);

			GLCheck( glBindVertexArray(quadVao) );
			GLCheck( glBindBuffer(GL_ARRAY_BUFFER, quadsDataVBO) );
			GLCheck( glBufferData(GL_ARRAY_BUFFER, nrOfInstances * sizeof(QuadData), quadsData, GL_STATIC_DRAW) );

			GLCheck( glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, (GLsizei)nrOfInstances) );

			if(isDebugCountingActive)
				++debugNumbers.drawCalls;
		};

		for(size_t quadIndex = 0; quadIndex < rg.quadsDataSize; ++quadIndex)
		{
			if(quadIndex == rg.quadsDataSize - 1)
			{
				bindTexturesForNextDrawCall();
				drawCall(quadIndex + 1);
				break;
			}
			else if(rg.quadsData[quadIndex + 1].textureSlotRef == 32)
			{
				bindTexturesForNextDrawCall();
				drawCall(quadIndex + 1);

				quadsData += quadIndex;

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

				quadIndex = 0;
			}
		}

		rg.quadsDataSize = 0;
		rg.texturesSize = 0;
	}

	if(affectedByLight)
	{
		// display debug numbers
		ImGui::Begin("Quad Renderer debug info");

		ImGui::Text("allocations: %u", debugNumbers.allocations);
		ImGui::Text("chunks: %u", debugNumbers.chunks);
		ImGui::Text("frames to delete chunk VBOs: %u", debugNumbers.framesToDeleteChunkVBOs);
		ImGui::Text("render groups: %u", debugNumbers.renderGroups);
		ImGui::Text("render groups not affected by light: %u", debugNumbers.renderGroupsNotAffectedByLight);
		ImGui::Text("draw calls: %u", debugNumbers.drawCalls);
		ImGui::Text("drawn sprites: %u", debugNumbers.drawnSprites);
		ImGui::Text("drawn textures: %u", debugNumbers.drawnTextures);
		
		auto submitDebugArray = [](const char* name, const std::vector<unsigned>& v)
		{
			std::string str(name);
			str += ":";
			for(auto e : v)
			{
				str += " ";
				str += std::to_string(e);
			}
			ImGui::Text(str.c_str());
		};
		submitDebugArray("render groups sizes", debugNumbers.renderGroupsSizes);
		submitDebugArray("render groups z", debugNumbers.renderGroupsZ);
		submitDebugArray("render groups indices", debugNumbers.renderGroupsIndices);
		submitDebugArray("no light render groups sizes", debugNumbers.notAffectedByLightRenderGroupsSizes);
		submitDebugArray("no light render groups z", debugNumbers.notAffectedByLightRenderGroupsZ);
		submitDebugArray("no light render groups indices", debugNumbers.notAffectedByLightRenderGroupsIndices);

		ImGui::End();
	}
}

}
