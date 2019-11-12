#include <GL/glew.h>
#include "renderer.hpp"
#include "EfficiencyRegister/efficiencyRegister.hpp"
#include "Logs/logs.hpp"
#include "openglErrors.hpp"
#include "framebuffer.hpp"
#include <SFML/Graphics/Transform.hpp>
#include <vector>
#include <algorithm>

namespace {
	// SceneData
	ph::FloatRect screenBounds;
	const float* viewProjectionMatrix = nullptr;

	// RendererData
	unsigned numberOfDrawCalls = 0;
	ph::Shader* defaultFramebufferShader;
	ph::Shader* defaultSpriteShader;
	ph::Shader* defaultInstanedSpriteShader;
	const ph::Shader* currentlyBoundShader = nullptr;
	ph::VertexArray* textureQuadVertexArray;
	ph::VertexArray* textureAnimatedQuadVertexArray;
	ph::VertexArray* framebufferVertexArray;
	ph::VertexArray* instancedQuadsVertexArray;
	ph::VertexBuffer instancedQuadsPositionsVBO;
	ph::Framebuffer* framebuffer;
	ph::Texture* whiteTexture;
	std::vector<sf::Vector2f> instancedSpritesPositions;
	std::vector<sf::Vector2f> instancedSpritesSizes;
	std::vector<sf::Color> instancedSpritesColors;
	std::vector<ph::FloatRect> instancedSpritesTextureRects;
	std::vector<int> instancedSpritesTextureSlotRefs;
	std::vector<const ph::Texture*> instancedTextures;
	bool isCustomTextureRectApplied = false;

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

	VertexBuffer quadVertexPositionsVBO = createVertexBuffer();
	setData(quadVertexPositionsVBO, quadPositions, sizeof(quadPositions), DataUsage::staticDraw);
	instancedQuadsVertexArray = new VertexArray;
	instancedQuadsVertexArray->setVertexBuffer(quadVertexPositionsVBO, VertexBufferLayout::position2);
	instancedQuadsVertexArray->setIndexBuffer(quadIBO);
	/*instancedQuadsPositionsVBO = createVertexBuffer();
	setData(instancedQuadsPositionsVBO, nullptr, 2 * sizeof(float) * 50000, DataUsage::dynamicDraw);
	GLCheck( glEnableVertexAttribArray(1) );
	bind(instancedQuadsPositionsVBO);
	GLCheck( glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), ( void*) 0) );
	GLCheck( glBindBuffer(GL_ARRAY_BUFFER, 0) );
	GLCheck( glVertexAttribDivisor(1, 1) );*/

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
	instancedSpritesPositions.reserve(1000);
	instancedSpritesSizes.reserve(1000);
	instancedSpritesColors.reserve(1000);
	instancedSpritesTextureRects.reserve(1000);
	instancedSpritesTextureSlotRefs.reserve(1000);

	// set up texture slots in default instanced sprite shader
	/*for(int i = 0; i < 16; ++i)
		defaultInstanedSpriteShader->setUniformInt("texture" + std::to_string(i), i);*/
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
	insFlush();

	Framebuffer::bindDefaultFramebuffer();
	GLCheck( glClear(GL_COLOR_BUFFER_BIT) );
	framebufferVertexArray->bind();
	defaultFramebufferShader->bind();
	currentlyBoundShader = defaultFramebufferShader;
	framebuffer->bindTextureColorBuffer(0);
	GLCheck( glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0) );

	sfmlRenderer.drawSubmitedObjects(window);

	efficiencyRegister.setDrawCallsPerFrame(numberOfDrawCalls);
	numberOfDrawCalls = 0;
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
	++numberOfDrawCalls;
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

void Renderer::submitQuadIns(const Texture* texture, const IntRect* texCoords, const sf::Color* color, const Shader* shader,
                             sf::Vector2f position, sf::Vector2f size, float rotation)
{
	instancedSpritesPositions.emplace_back(position);
	instancedSpritesSizes.emplace_back(size);
	instancedSpritesColors.emplace_back(color ? *color : sf::Color::White);

	// TODO_ren: Add support for custom tex coords
	if(texCoords == nullptr)
		instancedSpritesTextureRects.emplace_back(FloatRect(0, 0, 1, 1));
	else
		PH_EXIT_GAME("Custom tex coords are not supported yet");

	if(shader)
		PH_EXIT_GAME("Custom shaders are not supported yet");

	// TODO_ren: Refactor this

	if(!texture)
		texture = whiteTexture;

	int textureSlotOfThisTexture = -1;
	for(size_t i = 0; i < instancedTextures.size(); ++i)
	{
		if(instancedTextures[i] == texture) {
			textureSlotOfThisTexture = i;
			break;
		}
	}

	if(textureSlotOfThisTexture == -1) // if texture was not assigned yet
	{
		if(instancedTextures.size() < 16) {
			const int textureSlotID = static_cast<int>(instancedTextures.size());
			instancedSpritesTextureSlotRefs.emplace_back(textureSlotID);
			texture->bind(textureSlotID);
			instancedTextures.emplace_back(texture);
		}
		else
			PH_EXIT_GAME("Add stuff here!"); // TODO_ren (flush)
	}
	else // if texture was already assigned
	{
		instancedSpritesTextureSlotRefs.emplace_back(textureSlotOfThisTexture);
	}
}

void Renderer::insFlush()
{
	if(defaultInstanedSpriteShader != currentlyBoundShader) {
		defaultInstanedSpriteShader->bind();
		currentlyBoundShader = defaultInstanedSpriteShader;
	}
	defaultInstanedSpriteShader->setUniformMatrix4x4("viewProjectionMatrix", viewProjectionMatrix);

	for(size_t i = 0; i < instancedSpritesPositions.size(); ++i)
		defaultInstanedSpriteShader->setUniformVector2("offsets[" + std::to_string(i) + "]", instancedSpritesPositions[i]);

	for(size_t i = 0; i < instancedSpritesSizes.size(); ++i)
		defaultInstanedSpriteShader->setUniformVector2("sizes[" + std::to_string(i) + "]", instancedSpritesSizes[i]);

	for(size_t i = 0; i < instancedSpritesColors.size(); ++i)
		defaultInstanedSpriteShader->setUniformVector4Color("colors[" + std::to_string(i) + "]", instancedSpritesColors[i]);

	for(size_t i = 0; i < instancedSpritesTextureRects.size(); ++i)
		defaultInstanedSpriteShader->setUniformVector4Rect("textureRects[" + std::to_string(i) + "]", instancedSpritesTextureRects[i]);
	
	for(size_t i = 0; i < instancedSpritesTextureSlotRefs.size(); ++i)
		defaultInstanedSpriteShader->setUniformInt("textureSlotRefs[" + std::to_string(i) + "]", instancedSpritesTextureSlotRefs[i]);

	// TODO_ren: Maybe I can set it once in Renderer::init()
	for(size_t i = 0; i < instancedTextures.size(); ++i)
		defaultInstanedSpriteShader->setUniformInt("textures[" + std::to_string(i) + "]", i);

	GLCheck( glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, instancedSpritesPositions.size()) );

	++numberOfDrawCalls;

	instancedSpritesPositions.clear();
	instancedSpritesSizes.clear();
	instancedSpritesColors.clear();
	instancedSpritesTextureRects.clear();
	instancedSpritesTextureSlotRefs.clear();
	instancedTextures.clear();
}

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

bool Renderer::isInsideScreen(sf::Vector2f position, sf::Vector2i size)
{
	return isInsideScreen(sf::FloatRect(position.x, position.y, static_cast<float>(size.x), static_cast<float>(size.y)));
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
