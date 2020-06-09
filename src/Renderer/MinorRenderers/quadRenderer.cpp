#include "pch.hpp"
#include "quadRenderer.hpp"
#include "Renderer/API/texture.hpp"
#include "Renderer/API/openglErrors.hpp"
#include "Renderer/Shaders/embeddedShaders.hpp"
#include "Utilities/cast.hpp"

extern bool debugWindowOpen;

namespace ph::QuadRenderer {

static constexpr u32 deleteVBOsDelay = 500;

struct DebugNumbers
{
	std::vector<u32> renderGroupsSizes; 
	std::vector<u32> renderGroupsZ; 
	std::vector<u32> renderGroupsIndices; 
	std::vector<u32> notAffectedByLightRenderGroupsSizes; 
	std::vector<u32> notAffectedByLightRenderGroupsZ; 
	std::vector<u32> notAffectedByLightRenderGroupsIndices; 
	u32 allocations = 0;
	u32 chunks = 0;
	u32 framesToDeleteChunkVBOs = 0;
	u32 renderGroups = 0;
	u32 renderGroupsNotAffectedByLight = 0;
	u32 drawCalls = 0;
	u32 drawnSprites = 0;
	u32 drawnTextures = 0;
};

struct RenderGroupsHashMap
{
	u32 capacity;
	u32 size;
	u32* indices = Null;
	RenderGroupKey* keys = Null;
	QuadRenderGroup* renderGroups = Null;
	bool needsToBeSorted = false;
};

struct GroundChunk
{
	FloatRect textureRect;
	Vec2 pos;
	sf::Color color;
	float z;
};

struct Chunk
{
	FloatRect bounds;
	u32 vbo;
	u32 quadsCount;
	sf::Color color;
	float z;
};

struct RegisteredChunk
{
	FloatRect bounds;
	u32 id;
	u32 vbo;
};

struct ChunksData
{
	std::vector<RegisteredChunk> registerChunks;
	std::vector<Chunk> thisFrameChunks;
	u32 nextRegisteredChunkID = 0;
	u32 dummyVAO;
	u32 framesToDeleteChunkVBOs = deleteVBOsDelay;
};

static ChunksData chunks;

static std::vector<GroundChunk> groundChunks;
static u32 groundChunkVAO; 

static u32 chunksTexture; // for chunks and ground chunks

static RenderGroupsHashMap renderGroupsHashMap;
static RenderGroupsHashMap notAffectedByLightRenderGroupsHashMap;

static DebugNumbers debugNumbers;
static bool debugColors = false;

static Shader defaultQuadShader;
static Shader groundChunkShader;
static Shader chunkShader;

static const FloatRect* screenBounds; 
static Texture* whiteTexture;
static u32 quadIBO;
static u32 quadsDataVBO;
static u32 quadVao;

void setChunksTexture(u32 texture)
{
	chunksTexture = texture;
}

DebugNumbers getDebugNumbers()
{
	return debugNumbers;
}

void setGameWorldCameraBoundsPtr(const FloatRect* bounds)
{
	screenBounds = bounds;		
}

static u32 bumpToNext4000(u32 size)
{
	u32 temp = size;
	while(temp > 4000)
		temp -= 4000;
	return size + 4000 - temp;	
}

static bool operator==(const RenderGroupKey& lhs, const RenderGroupKey& rhs)
{
	return lhs.shader == rhs.shader && lhs.z == rhs.z;
}

static QuadRenderGroup* insertIfDoesNotExitstAndGetRenderGroup(RenderGroupsHashMap* hashMap, RenderGroupKey key, u32 quadDataCount)
{
	// return render group of matching key if exists
	for(u32 i = 0; i < hashMap->size; ++i)
		if(key == *(hashMap->keys + i))
			return hashMap->renderGroups + i;

	// bump debug numbers
	if(hashMap == &renderGroupsHashMap)
	{
		debugNumbers.renderGroups = hashMap->size;
		debugNumbers.renderGroupsZ.emplace_back(Cast<u32>(key.z * 255));
	}
	else if(hashMap == &notAffectedByLightRenderGroupsHashMap)
	{
		debugNumbers.renderGroupsNotAffectedByLight = hashMap->size; 
		debugNumbers.notAffectedByLightRenderGroupsZ.emplace_back(Cast<u32>(key.z * 255));
	}

	// reallocate if there is no more space
	if(hashMap->capacity == hashMap->size)
	{
		hashMap->capacity *= 2;

		void* newIndices = realloc(hashMap->indices, hashMap->capacity * sizeof(u32));
		PH_ASSERT_CRITICAL(newIndices, "quad render group hash map indices realloc failed!");
		hashMap->indices = Cast<u32*>(newIndices);

		void* newKeys = realloc(hashMap->keys, hashMap->capacity * sizeof(RenderGroupKey));
		PH_ASSERT_CRITICAL(newKeys, "quad render group hash map keys realloc failed!");
		hashMap->keys = Cast<RenderGroupKey*>(newKeys);

		void* newRenderGroups = realloc(hashMap->renderGroups, hashMap->capacity * sizeof(QuadRenderGroup));
		PH_ASSERT_CRITICAL(newRenderGroups, "quad render group hash map render groups realloc failed!");
		hashMap->renderGroups = Cast<QuadRenderGroup*>(newRenderGroups);

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
	qrg.quadsDataArenaSize = Cast<u32>(arenaSize); 
	qrg.texturesSize = 0;
	qrg.texturesCapacity = 32;
	qrg.textures = Cast<u32*>(malloc(sizeof(u32) * 32));
	qrg.quadsDataSize = 0; 
	qrg.quadsDataCapacity = quadDataCount; 
	qrg.quadsData = Cast<QuadData*>(malloc(arenaSize));
	debugNumbers.allocations += 2;

	++hashMap->size;

	return &qrg; 
}

static auto getTextureSlotToWhichThisTextureIsBound(const Texture* texture, QuadRenderGroup* rg) -> std::optional<float>
{
	u32* ptr = rg->textures;
	for(size_t i = 0; i < rg->texturesSize; ++i, ++ptr)
		if(*ptr == texture->getID())
			return Cast<float>(i);
	return std::nullopt;
}

static void insertQuadDataToQuadRenderGroup(QuadData* quadData, u32 count, QuadRenderGroup* quadRenderGroup)
{
	bool thereIsNoPlaceForNewQuadData = quadRenderGroup->quadsDataSize + count > quadRenderGroup->quadsDataCapacity;
	if(thereIsNoPlaceForNewQuadData)
	{
		// reallocate quad render group arena
		size_t oldArenaSize = (size_t)quadRenderGroup->quadsDataArenaSize; 
		void* oldArena = (void*)quadRenderGroup->quadsData; 
		u32 newQuadsDataCapacity = quadRenderGroup->quadsDataCapacity + count * 2; 
		size_t newArenaSize = newQuadsDataCapacity * sizeof(QuadData); 
		quadRenderGroup->quadsDataArenaSize = (u32)newArenaSize;
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
	hashMap.indices = Cast<u32*>(malloc(sizeof(u32) * initialGroupsCapacity));
	hashMap.keys = Cast<RenderGroupKey*>(malloc(sizeof(RenderGroupKey) * initialGroupsCapacity));
	hashMap.renderGroups = Cast<QuadRenderGroup*>(malloc(sizeof(QuadRenderGroup) * initialGroupsCapacity));
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

	for(i32 i = 0; i < 7; ++i) {
		GLCheck( glEnableVertexAttribArray(i) );
	}
	for(i32 i = 0; i < 7; ++i) {
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
	u32 groundChunkVBO;
	glGenBuffers(1, &groundChunkVBO);
	glBindBuffer(GL_ARRAY_BUFFER, groundChunkVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(groundChunkVertices), groundChunkVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), Null);

	// create white texture
	whiteTexture = new Texture;
	u32 white = 0xffffffff;
	whiteTexture->setData(&white, sizeof(u32), Vec2i(1, 1));
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

void submitGroundChunk(Vec2 pos, const FloatRect& textureRect, float z, sf::Color color)
{
	groundChunks.push_back(GroundChunk{textureRect, pos, color, z});
}

u32 registerNewChunk(const FloatRect& bounds)
{
	chunks.registerChunks.emplace_back(RegisteredChunk{bounds, chunks.nextRegisteredChunkID, 0});
	++chunks.nextRegisteredChunkID;
	return chunks.nextRegisteredChunkID - 1;
}

void submitChunk(std::vector<ChunkQuadData>& quadsData, const FloatRect& bounds,
                 float z, u32* id, sf::Color color)
{
	for(auto& cached : chunks.registerChunks)
	{
		if(*id == cached.id) 
		{
			if(cached.vbo)
			{
				chunks.thisFrameChunks.emplace_back(Chunk{bounds, cached.vbo, (u32)quadsData.size(), color, z});
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
	QuadRenderGroup* renderGroup = insertIfDoesNotExitstAndGetRenderGroup(&hashMap, {shader, z, projectionType}, Cast<u32>(quadsData.size()));

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

	insertQuadDataToQuadRenderGroup(quadsData.data(), Cast<u32>(quadsData.size()), renderGroup);
}

void submitQuad(Texture* texture, const IntRect* textureRect, const sf::Color* color, const Shader* shader,
                Vec2 pos, Vec2 size, float z, float rotation, Vec2 rotationOrigin,
                ProjectionType projectionType, bool isAffectedByLight)
{
	// culling
	FloatRect bounds = projectionType == ProjectionType::gameWorld ? *screenBounds : FloatRect(0.f, 0.f, 1920.f, 1080.f);
	if(rotation == 0.f)
		if(!intersect(bounds, FloatRect(pos.x, pos.y, size.x, size.y)))
			return;
	else if(!intersect(bounds, FloatRect(pos.x - size.x * 2, pos.y - size.y * 2, size.x * 4, size.y * 4)))
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
		auto ts = Cast<Vec2>(texture->getSize());
		finalTextureRect = FloatRect(
			textureRect->x / ts.x, (ts.y - textureRect->y - textureRect->h) / ts.y,
			textureRect->w / ts.x, textureRect->h / ts.y
		);
	}
	else
	{
		finalTextureRect = FloatRect(0.f, 0.f, 1.f, 1.f);	
	}

	// submit data
	QuadData quadData;
	
	quadData.color = color ? castToNormalizedColorVec4(*color) : castToNormalizedColorVec4(sf::Color::White);
	quadData.textureRect = finalTextureRect; 
	quadData.position = pos;
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
		u32 textureSlotID = renderGroup->texturesSize;
		quadData.textureSlotRef = (float)textureSlotID;
		*(renderGroup->textures + textureSlotID) = texture->getID();
		++renderGroup->texturesSize;
	}

	insertQuadDataToQuadRenderGroup(&quadData, 1, renderGroup);
}

void flush(bool affectedByLight)
{
	PH_PROFILE_FUNCTION();

	const Shader* currentlyBoundShader = Null;
	auto& hashMap = affectedByLight ? renderGroupsHashMap : notAffectedByLightRenderGroupsHashMap;

	// sort hash map indices
	if(hashMap.needsToBeSorted)
	{
		PH_PROFILE_SCOPE("sorting indices");
		hashMap.needsToBeSorted = false;
		for(u32 i = 0; i < hashMap.size - 1; ++i)
		{
			for(u32 j = 0; j < hashMap.size - 1; ++j)
			{
				u32 index1 = hashMap.indices[j];
				u32 index2 = hashMap.indices[j + 1];
				if(hashMap.keys[index1].z < hashMap.keys[index2].z)
				{
					u32 temp = hashMap.indices[j];
					hashMap.indices[j] = hashMap.indices[j + 1];
					hashMap.indices[j + 1] = temp; 
				}
			}
		}
		
		debugNumbers.renderGroupsIndices.clear();
		auto& debugIndices = affectedByLight ? debugNumbers.renderGroupsIndices : debugNumbers.notAffectedByLightRenderGroupsIndices;

		/* TODO
		for(u32 i = 0; i < hashMap.size; ++i)
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

		for(u32 chunkIndex = 0, groundChunkIndex = 0;
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
				groundChunkShader.setUniformVec2("chunkPos", gc.pos);
				groundChunkShader.setUniformFloat("z", gc.z);
				groundChunkShader.setUniformVec4Color("color", gc.color);
				groundChunkShader.setUniformVec2("uvTopLeft", gc.textureRect.pos);
				groundChunkShader.setUniformVec2("uvTopRight", gc.textureRect.topRight());
				groundChunkShader.setUniformVec2("uvBottomLeft", gc.textureRect.bottomLeft());
				groundChunkShader.setUniformVec2("uvBottomRight", gc.textureRect.bottomRight());

				GLCheck( glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, 144) );

				++groundChunkIndex;
				
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
				chunkShader.setUniformVec4Color("color", chunk.color);
				chunkShader.setUniformFloat("z", chunk.z);

				GLCheck( glBindVertexArray(chunks.dummyVAO) );
				GLCheck( glBindBuffer(GL_ARRAY_BUFFER, chunk.vbo) );

				GLCheck( glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(ChunkQuadData), (void*)offsetof(ChunkQuadData, textureRect)) );
				GLCheck( glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(ChunkQuadData), (void*)offsetof(ChunkQuadData, position)) );
				GLCheck( glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ChunkQuadData), (void*)offsetof(ChunkQuadData, size)) );
				GLCheck( glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(ChunkQuadData), (void*)offsetof(ChunkQuadData, rotation)) );

				for(i32 i = 0; i < 4; ++i)
					glEnableVertexAttribArray(i);

				for(i32 i = 0; i < 4; ++i)
					glVertexAttribDivisor(i, 1);

				GLCheck( glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, chunk.quadsCount) );

				++chunkIndex;

				++debugNumbers.chunks; 
				++debugNumbers.drawCalls;
			}
		}

		if(--chunks.framesToDeleteChunkVBOs == 0)
		{
			// delete vbos of chunks outside camera 

			chunks.framesToDeleteChunkVBOs = deleteVBOsDelay;

			for(auto& cached : chunks.registerChunks)
			{
				if(cached.vbo && !intersect(*screenBounds, cached.bounds))
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
	for(u32 pairIndex = 0; pairIndex < hashMap.size; ++pairIndex)
	{
		PH_PROFILE_SCOPE("draw hash map pair");
		u32 renderGroupIndex = hashMap.indices[pairIndex];
		auto& key = hashMap.keys[renderGroupIndex];
		auto& rg = hashMap.renderGroups[renderGroupIndex];

		// update debug info
		debugNumbers.drawnSprites += rg.quadsDataSize;
		debugNumbers.drawnTextures += rg.texturesSize;
		if(affectedByLight)
			debugNumbers.renderGroupsSizes.emplace_back(rg.quadsDataSize);
		else
			debugNumbers.notAffectedByLightRenderGroupsSizes.emplace_back(rg.quadsDataSize);

		// set up shader
		if(key.shader != currentlyBoundShader) 
		{
			key.shader->bind();
			currentlyBoundShader = key.shader;

			i32 textures[32];
			for(i32 i = 0; i < 32; ++i)
				textures[i] = i;
			key.shader->setUniformI32Array("textures", 32, textures);
		}
		key.shader->setUniformFloat("z", key.z);
		key.shader->setUniformBool("isGameWorldProjection", key.projectionType == ProjectionType::gameWorld);

		// TODO: sort quads by texture slot ref if there are more then 32 
		// std::sort(rg.quadsData.begin(), rg.quadsData.end(), [](const QuadData& a, const QuadData& b) { return a.textureSlotRef < b.textureSlotRef; });

		// draw render group
		u32 quadsDataSize = rg.quadsDataSize;
		u32 texturesSize = rg.texturesSize;
		QuadData* quadsData = rg.quadsData;
		u32* textures = rg.textures;

		auto bindTexturesForNextDrawCall = [textures, texturesSize]
		{
			for(u32 textureSlot = 0; textureSlot < (texturesSize > 32 ? 32 : texturesSize); ++textureSlot)
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
				for(u32 i = 0; i < quadsDataSize; ++i)
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
}

void submitDebug()
{
	if(ImGui::BeginTabItem("quad renderer"))
	{
		if(ImGui::Checkbox("colorful debug", &debugColors))
		{
			defaultQuadShader.bind();
			defaultQuadShader.setUniformBool("debugVisualization", debugColors);

			chunkShader.bind();
			chunkShader.setUniformBool("debugVisualization", debugColors);

			groundChunkShader.bind();
			groundChunkShader.setUniformBool("debugVisualization", debugColors);
		}
		if(debugColors)
		{
			ImGui::Text("red - ground chunk");
			ImGui::Text("green - chunk");
			ImGui::Text("blue - hash map");
			ImGui::Separator();
		}

		ImGui::Text("allocations: %u", debugNumbers.allocations);
		ImGui::Text("chunks: %u", debugNumbers.chunks);
		ImGui::Text("frames to delete chunk VBOs: %u", debugNumbers.framesToDeleteChunkVBOs);
		ImGui::Text("render groups: %u", debugNumbers.renderGroups);
		ImGui::Text("render groups not affected by light: %u", debugNumbers.renderGroupsNotAffectedByLight);
		ImGui::Text("draw calls: %u", debugNumbers.drawCalls);
		ImGui::Text("drawn sprites: %u", debugNumbers.drawnSprites);
		ImGui::Text("drawn textures: %u", debugNumbers.drawnTextures);
		
		auto submitDebugArray = [](const char* name, const std::vector<u32>& v)
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

		ImGui::EndTabItem();
	}
	debugNumbers.renderGroupsSizes.clear(); 
	debugNumbers.notAffectedByLightRenderGroupsSizes.clear(); 
	debugNumbers.drawCalls = 0;
	debugNumbers.chunks = 0;
	debugNumbers.drawnSprites = 0;
	debugNumbers.drawnTextures = 0;
}

}
