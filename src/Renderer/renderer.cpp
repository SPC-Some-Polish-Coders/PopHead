#include <GL/glew.h>
#include "renderer.hpp"
#include "EfficiencyRegister/efficiencyRegister.hpp"
#include "Logs/logs.hpp"
#include "openglErrors.hpp"
#include "framebuffer.hpp"
#include "Utilities/vector4.hpp"
#include "Utilities/cast.hpp"
#include <SFML/Graphics/Transform.hpp>
#include <vector>
#include <algorithm>

namespace {
	// SceneData
	ph::FloatRect screenBounds;
	const float* viewProjectionMatrix = nullptr;

	// RendererData
	unsigned numberOfDrawCalls = 0;
	unsigned numberOfDrawnSprites = 0;

	ph::Shader* defaultFramebufferShader;
	ph::Shader* defaultSpriteShader;
	ph::Shader* defaultInstanedSpriteShader;
	const ph::Shader* currentlyBoundShader = nullptr;
	
	ph::VertexArray* textureQuadVertexArray;
	ph::VertexArray* textureAnimatedQuadVertexArray;
	ph::VertexArray* framebufferVertexArray;
	ph::VertexArray* instancedQuadsVertexArray;
	
	ph::Framebuffer* framebuffer;
	
	ph::Texture* whiteTexture;
	
	std::vector<sf::Vector2f> instancedSpritesPositions;
	std::vector<sf::Vector2f> instancedSpritesSizes;
	std::vector<float> instancedSpritesRotations;
	std::vector<ph::Vector4f> instancedSpritesColors;
	std::vector<ph::FloatRect> instancedSpritesTextureRects;
	std::vector<int> instancedSpritesTextureSlotRefs;
	std::vector<const ph::Texture*> instancedTextures;
	
	unsigned instancedPositionsVBO;
	unsigned instancedSizesVBO;
	unsigned instancedRotationsVBO;
	unsigned instancedColorsVBO;
	unsigned instancedTextureRectsVBO;
	unsigned instancedVAO;

	constexpr int nrOfSpritesInOneInstancedDrawCall = 2500;

	// TODO_ren: Get rid of SFML Renderer
	ph::SFMLRenderer sfmlRenderer;
}

namespace ph {

void Renderer::init(unsigned screenWidth, unsigned screenHeight)
{
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK)
		PH_EXIT_GAME("GLEW wasn't initialized correctly!");

	// set up blending
	GLCheck( glEnable(GL_BLEND) );
	GLCheck( glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) );

	// load default shaders
	auto& sl = ShaderLibrary::getInstance();
	sl.loadFromFile("sprite", "resources/shaders/sprite.vs.glsl", "resources/shaders/sprite.fs.glsl");
	defaultSpriteShader = sl.get("sprite");
	sl.loadFromFile("instancedSprite", "resources/shaders/instancedSprite.vs.glsl", "resources/shaders/instancedSprite.fs.glsl");
	defaultInstanedSpriteShader = sl.get("instancedSprite");
	sl.loadFromFile("defaultFramebuffer", "resources/shaders/defaultFramebuffer.vs.glsl", "resources/shaders/defaultFramebuffer.fs.glsl");
	defaultFramebufferShader = sl.get("defaultFramebuffer");

	// load quad vertex arrays
	float quadPositionsAndTextureCoords[] = {
		1.f, 0.f, 1.f, 1.f,
		1.f, 1.f, 1.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 0.f, 1.f 
	};

	float framebufferQuad[] = {
		1.f,-1.f, 1.f, 0.f,
		1.f, 1.f, 1.f, 1.f,
	   -1.f, 1.f, 0.f, 1.f,
	   -1.f,-1.f, 0.f, 0.f
	};

	float quadPositions[] = {
		1.f, 0.f,
		1.f, 1.f,
		0.f, 1.f,
		0.f, 0.f
	};

	unsigned quadIndices[] = { 0, 1, 3, 1, 2, 3 };
	IndexBuffer quadIBO = createIndexBuffer();
	setData(quadIBO, quadIndices, sizeof(quadIndices));
	
	VertexBuffer textureQuadVBO = createVertexBuffer();
	setData(textureQuadVBO, quadPositionsAndTextureCoords, sizeof(quadPositionsAndTextureCoords), DataUsage::staticDraw);
	textureQuadVertexArray = new VertexArray;
	textureQuadVertexArray->setVertexBuffer(textureQuadVBO, VertexBufferLayout::position2_texCoords2);
	textureQuadVertexArray->setIndexBuffer(quadIBO);

	VertexBuffer animatedTextureQuadVBO = createVertexBuffer();
	setData(animatedTextureQuadVBO, nullptr, sizeof(quadPositionsAndTextureCoords), DataUsage::dynamicDraw);
	textureAnimatedQuadVertexArray = new VertexArray;
	textureAnimatedQuadVertexArray->setVertexBuffer(animatedTextureQuadVBO, VertexBufferLayout::position2_texCoords2);
	textureAnimatedQuadVertexArray->setIndexBuffer(quadIBO);

	// create instanced arrays on gpu side
	{
		// TODO_ren: Try to pack it into one struct

		glGenVertexArrays(1, &instancedVAO);
		glBindVertexArray(instancedVAO);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadIBO.mID);

		glGenBuffers(1, &instancedPositionsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, instancedPositionsVBO);
		glBufferData(GL_ARRAY_BUFFER, nrOfSpritesInOneInstancedDrawCall * sizeof(sf::Vector2f), nullptr, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) 0);
		glVertexAttribDivisor(0, 1);

		glGenBuffers(1, &instancedSizesVBO);
		glBindBuffer(GL_ARRAY_BUFFER, instancedSizesVBO);
		glBufferData(GL_ARRAY_BUFFER, nrOfSpritesInOneInstancedDrawCall * sizeof(sf::Vector2f), nullptr, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), ( void*) 0);
		glVertexAttribDivisor(1, 1);

		glGenBuffers(1, &instancedRotationsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, instancedRotationsVBO);
		glBufferData(GL_ARRAY_BUFFER, nrOfSpritesInOneInstancedDrawCall * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(float), ( void*) 0);
		glVertexAttribDivisor(2, 1);

		glGenBuffers(1, &instancedColorsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, instancedColorsVBO);
		glBufferData(GL_ARRAY_BUFFER, nrOfSpritesInOneInstancedDrawCall * 4 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), ( void*) 0);
		glVertexAttribDivisor(3, 1);

		glGenBuffers(1, &instancedTextureRectsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, instancedTextureRectsVBO);
		glBufferData(GL_ARRAY_BUFFER, nrOfSpritesInOneInstancedDrawCall * 4 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) 0);
		glVertexAttribDivisor(4, 1);
	}

	VertexBuffer framebufferVBO = createVertexBuffer();
	setData(framebufferVBO, framebufferQuad, sizeof(framebufferQuad), DataUsage::staticDraw);
	framebufferVertexArray = new VertexArray;
	framebufferVertexArray->setVertexBuffer(framebufferVBO, VertexBufferLayout::position2_texCoords2);
	framebufferVertexArray->setIndexBuffer(quadIBO);

	// set up framebuffer
	framebuffer = new Framebuffer(screenWidth, screenHeight);

	whiteTexture = new Texture;
	unsigned whiteData = 0xffffffff;
	whiteTexture->setData(&whiteData, sizeof(unsigned), sf::Vector2i(1, 1));

	// allocate instanced vectors
	instancedSpritesPositions.reserve(nrOfSpritesInOneInstancedDrawCall);
	instancedSpritesSizes.reserve(nrOfSpritesInOneInstancedDrawCall);
	instancedSpritesRotations.reserve(nrOfSpritesInOneInstancedDrawCall);
	instancedSpritesColors.reserve(nrOfSpritesInOneInstancedDrawCall);
	instancedSpritesTextureRects.reserve(nrOfSpritesInOneInstancedDrawCall);
	instancedSpritesTextureSlotRefs.reserve(nrOfSpritesInOneInstancedDrawCall);

	defaultInstanedSpriteShader->bind();
	for(size_t i = 0; i < 32; ++i)
		defaultInstanedSpriteShader->setUniformInt("textures[" + std::to_string(i) + "]", i);
}

void Renderer::reset(unsigned screenWidth, unsigned screenHeight)
{
	shutDown();
	init(screenWidth, screenHeight);
}

void Renderer::shutDown()
{
	delete textureQuadVertexArray;
	delete textureAnimatedQuadVertexArray;
	delete framebufferVertexArray;
	delete framebuffer;
	delete whiteTexture;

	// TODO_ren: It's missing instanced arrays
}

void Renderer::beginScene(Camera& camera)
{
	framebuffer->bind();

	GLCheck( glClear(GL_COLOR_BUFFER_BIT) );

	viewProjectionMatrix = camera.getViewProjectionMatrix4x4().getMatrix();
	
	const sf::Vector2f center = camera.getCenter();
	const sf::Vector2f size = camera.getSize();
	screenBounds = FloatRect(center.x - size.x / 2, center.y - size.y / 2, size.x, size.y);
}

void Renderer::endScene(sf::RenderWindow& window, EfficiencyRegister& efficiencyRegister)
{
	flushInstancedSprites();

	Framebuffer::bindDefaultFramebuffer();
	GLCheck( glClear(GL_COLOR_BUFFER_BIT) );
	framebufferVertexArray->bind();
	defaultFramebufferShader->bind();
	currentlyBoundShader = defaultFramebufferShader;
	framebuffer->bindTextureColorBuffer(0);
	GLCheck( glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0) );

	int nrOfRenderedSFMLObjects;
	sfmlRenderer.drawSubmitedObjects(window, nrOfRenderedSFMLObjects);
	numberOfDrawCalls += nrOfRenderedSFMLObjects;
	numberOfDrawnSprites += nrOfRenderedSFMLObjects;

	efficiencyRegister.setDrawCallsPerFrame(numberOfDrawCalls);
	numberOfDrawCalls = 0;
	efficiencyRegister.setNumberOfDrawnSprites(numberOfDrawnSprites);
	numberOfDrawnSprites = 0;
}

void Renderer::submitQuad(const Texture* texture, const IntRect* textureRect, const sf::Color* color , const Shader* shader,
                          sf::Vector2f position, sf::Vector2i size, float rotation)
{
	// culling
	if(!isInsideScreen(position, size))
		return;

	// shader
	if(!shader)
		shader = defaultSpriteShader;
	if(shader != currentlyBoundShader) {
		shader->bind();
		currentlyBoundShader = shader;
	}
	if(!color)
		shader->setUniformVector4Color("color", sf::Color::White);
	else
		shader->setUniformVector4Color("color", *color);
	setQuadTransformUniforms(shader, position, size, rotation);

	// texture
	if(texture){
		if(textureRect) {
			setTextureRect(textureAnimatedQuadVertexArray->getVertexBuffer(), *textureRect, texture->getSize());
			textureAnimatedQuadVertexArray->bind();
		}
		else
			textureQuadVertexArray->bind();

		texture->bind();
	}
	else {
		textureQuadVertexArray->bind();
		whiteTexture->bind();
	}

	// draw call
	GLCheck(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

	// set debug data
	++numberOfDrawCalls;
	numberOfDrawnSprites += instancedSpritesPositions.size();
}

void Renderer::setQuadTransformUniforms(const Shader* shader, sf::Vector2f position, const sf::Vector2i size, float rotation)
{
	sf::Transform transform;
	transform.translate(position);
	transform.scale(static_cast<sf::Vector2f>(size));
	if(rotation != 0.f)
		transform.rotate(rotation);
	shader->setUniformMatrix4x4("modelMatrix", transform.getMatrix());
	shader->setUniformMatrix4x4("viewProjectionMatrix", viewProjectionMatrix);
	// TODO_ren: Does viewProjectionMatrix have to be set for each object even if we don't change shader
}

// TODO_ren: Support custom shaders for instanced rendering

void Renderer::submitQuad(const Texture* texture, const IntRect* texCoords, const sf::Color* color, 
                          sf::Vector2f position, sf::Vector2f size, float rotation)
{
	if(!isInsideScreen(position, size))
		return;

	instancedSpritesPositions.emplace_back(position);
	instancedSpritesSizes.emplace_back(size);
	instancedSpritesRotations.emplace_back(rotation);
	instancedSpritesColors.emplace_back(color ? Cast::toNormalizedColorVector4f(*color) : Cast::toNormalizedColorVector4f(sf::Color::White));

	// TODO_ren: Add support for custom tex coords
	if(texCoords == nullptr)
		instancedSpritesTextureRects.emplace_back(FloatRect(0, 0, 1, 1));
	else
		PH_EXIT_GAME("Custom tex coords are not supported yet");

	if(!texture)
		texture = whiteTexture;

	auto textureSlotOfThisTexture = getTextureSlotToWhichThisTextureIsBound(texture);
	if(textureSlotOfThisTexture)
		instancedSpritesTextureSlotRefs.emplace_back(*textureSlotOfThisTexture);
	else {
		if(instancedTextures.size() < 32) {
			const int textureSlotID = static_cast<int>(instancedTextures.size());
			instancedSpritesTextureSlotRefs.emplace_back(textureSlotID);
			texture->bind(textureSlotID);
			instancedTextures.emplace_back(texture);
		}
		else
			PH_EXIT_GAME("Add stuff here!"); // TODO_ren (flush)
	}
}

auto Renderer::getTextureSlotToWhichThisTextureIsBound(const Texture* texture) -> std::optional<int>
{
	for(size_t i = 0; i < instancedTextures.size(); ++i)
		if(instancedTextures[i] == texture)
			return i;
	return std::nullopt;
}

void Renderer::flushInstancedSprites()
{
	if(defaultInstanedSpriteShader != currentlyBoundShader) {
		defaultInstanedSpriteShader->bind();
		currentlyBoundShader = defaultInstanedSpriteShader;
	}
	defaultInstanedSpriteShader->setUniformMatrix4x4("viewProjectionMatrix", viewProjectionMatrix);

	glBindBuffer(GL_ARRAY_BUFFER, instancedPositionsVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, instancedSpritesPositions.size() * sizeof(sf::Vector2f), instancedSpritesPositions.data());

	glBindBuffer(GL_ARRAY_BUFFER, instancedSizesVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, instancedSpritesSizes.size() * sizeof(sf::Vector2f), instancedSpritesSizes.data());
	
	glBindBuffer(GL_ARRAY_BUFFER, instancedRotationsVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, instancedSpritesRotations.size() * sizeof(float), instancedSpritesRotations.data());
	
	glBindBuffer(GL_ARRAY_BUFFER, instancedColorsVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, instancedSpritesColors.size() * 4 * sizeof(float), instancedSpritesColors.data());

	glBindBuffer(GL_ARRAY_BUFFER, instancedTextureRectsVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, instancedSpritesTextureRects.size() * 4 * sizeof(float), instancedSpritesTextureRects.data());
	
	for(size_t i = 0; i < instancedSpritesTextureSlotRefs.size(); ++i)
		defaultInstanedSpriteShader->setUniformInt("textureSlotRefs[" + std::to_string(i) + "]", instancedSpritesTextureSlotRefs[i]);

	glBindVertexArray(instancedVAO);
	GLCheck( glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, instancedSpritesPositions.size()) );

	++numberOfDrawCalls;
	numberOfDrawnSprites += instancedSpritesPositions.size();

	instancedSpritesPositions.clear();
	instancedSpritesSizes.clear();
	instancedSpritesRotations.clear();
	instancedSpritesColors.clear();
	instancedSpritesTextureRects.clear();
	instancedSpritesTextureSlotRefs.clear();
	instancedTextures.clear();
}

// TODO_ren: Think what to do with these low level kindof calls
void Renderer::submit(VertexArray& vao, Shader& shader, const sf::Transform& transform, const sf::Vector2i size, DrawPrimitive drawMode)
{
	if(!isInsideScreen(transform, size))
		return;

	vao.bind();

	// TODO_ren: Make that we don't bind shader if it was already bound
 	shader.bind();
	shader.setUniformMatrix4x4("modelMatrix", transform.getMatrix());
	shader.setUniformMatrix4x4("viewProjectionMatrix", viewProjectionMatrix);
	
	GLCheck( glDrawElements(toGLEnum(drawMode), vao.getIndexBuffer().mNumberOfIndices, GL_UNSIGNED_INT, 0) );
	
	++numberOfDrawCalls;
}

void Renderer::submit(VertexArray& vao, Shader& shader, const FloatRect bounds, DrawPrimitive drawMode)
{
	if(!isInsideScreen(bounds))
		return;

	vao.bind();

	// TODO_ren: Make that we don't bind shader if it was already bound
	shader.bind();
	shader.setUniformMatrix4x4("modelMatrix", sf::Transform::Identity.getMatrix());
	shader.setUniformMatrix4x4("viewProjectionMatrix", viewProjectionMatrix);

	GLCheck(glDrawElements(toGLEnum(drawMode), vao.getIndexBuffer().mNumberOfIndices, GL_UNSIGNED_INT, 0));

	++numberOfDrawCalls;
}

void Renderer::submit(VertexArray& vao, const FloatRect bounds, DrawPrimitive drawMode)
{
	submit(vao, *defaultSpriteShader, bounds, drawMode);
}

void Renderer::submit(VertexArray& vao, const sf::Transform& transform, const sf::Vector2i size, DrawPrimitive drawMode)
{
	submit(vao, *defaultSpriteShader, transform, size, drawMode);
}

void Renderer::submit(Sprite& sprite, Shader& shader, const sf::Transform& transform, DrawPrimitive drawMode)
{
	sprite.mTexture.bind();
	submit(sprite.mVertexArray, shader, transform, sprite.mSize, drawMode);
}

void Renderer::submit(Sprite& sprite, const sf::Transform& transform, DrawPrimitive drawMode)
{
	submit(sprite, *defaultSpriteShader, transform, drawMode);
}

void Renderer::submit(const sf::Drawable& object)
{
	sfmlRenderer.submit(&object);
	++numberOfDrawCalls;
}

bool Renderer::isInsideScreen(const sf::Transform& transform, const sf::Vector2i size)
{
	const FloatRect objectRect(transform.getMatrix()[12], transform.getMatrix()[13], static_cast<float>(size.x), static_cast<float>(size.y));
	return screenBounds.doPositiveRectsIntersect(objectRect);
}

// TODO_ren: Remove this sf::Vector2i version
bool Renderer::isInsideScreen(sf::Vector2f position, sf::Vector2i size)
{
	return isInsideScreen(position, static_cast<sf::Vector2f>(size));
}

bool Renderer::isInsideScreen(sf::Vector2f position, sf::Vector2f size)
{
	return isInsideScreen(sf::FloatRect(position.x, position.y, size.x, size.y));
}

bool Renderer::isInsideScreen(const FloatRect objectBounds)
{
	return screenBounds.doPositiveRectsIntersect(objectBounds);
}

void Renderer::onWindowResize(unsigned width, unsigned height)
{
	GLCheck( glViewport(0, 0, width, height) );
	framebuffer->reset(width, height);
}

void Renderer::setClearColor(const sf::Color& color)
{
	GLCheck( glClearColor(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f) );
}

}
